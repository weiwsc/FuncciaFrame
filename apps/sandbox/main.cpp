#include <exception>
#include <filesystem>
#include <charconv>
#include <iostream>
#include <optional>
#include <string_view>
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "core/CpuProfiler.h"
#include "core/FpsCounter.h"
#include "core/Log.h"
#include "gfx/vulkan/VulkanRenderer.h"
#include "platform/SDLWindow.h"

int main(int argc, char** argv) {
    try {
        bool shared_depth = false;
        bool no_model = false;
        bool validation = false;
        int benchmark_seconds = 0;
        std::optional<vk::PresentModeKHR> present_mode;
        for (int i = 1; i < argc; ++i) {
            const std::string_view arg = argv[i];
            if (arg == "--shared-depth") shared_depth = true;
            else if (arg == "--no-model") no_model = true;
            else if (arg == "--validation") validation = true;
            else if (arg == "--present=immediate") present_mode = vk::PresentModeKHR::eImmediate;
            else if (arg == "--present=mailbox") present_mode = vk::PresentModeKHR::eMailbox;
            else if (arg == "--present=fifo") present_mode = vk::PresentModeKHR::eFifo;
            else if (arg.starts_with("--benchmark-seconds=")) {
                const auto value = arg.substr(std::string_view("--benchmark-seconds=").size());
                const auto [end, error] = std::from_chars(value.data(), value.data() + value.size(), benchmark_seconds);
                if (error != std::errc{} || end != value.data() + value.size() ||
                    benchmark_seconds < 1 || benchmark_seconds > 3600) {
                    std::cerr << "benchmark duration must be an integer from 1 to 3600 seconds\n";
                    return 1;
                }
            } else {
                std::cerr << "Usage: sandbox [--shared-depth] [--no-model] [--validation] "
                             "[--present=immediate|mailbox|fifo] [--benchmark-seconds=10]\n";
                return arg == "--help" ? 0 : 1;
            }
        }
        vva::engine::log::init();

        vva::gfx::GL::SDLWindow window;
        if (!window.Initialize(1920 / 2, 1080 / 2, "FuncciaFrame", vva::gfx::RenderBackend::Vulkan)) {
            vva_log_critical("failed to initialize Vulkan window");
            vva::engine::log::shutdown();
            return 1;
        }
        vva::util::FpsCounter fps_counter;
        auto renderer = vva::gfx::vulkan::VulkanRenderer::createVulkanRenderer(window, {
            .app_name = "FuncciaFrame",
            .shader_dir = std::filesystem::path{VVA_ASSET_DIR} / "shaders",
            .enable_validation = validation,
            .per_frame_depth = !shared_depth,
            .present_mode = present_mode,
        });

        vva::gfx::Transform transform {};
        //transform.rotate(vva::gfx::Vector3::RIGHT,-140.f);
        transform.rotate(vva::gfx::Vector3::UP,90.f);
        

        std::optional<vva::gfx::vulkan::ModelHandle> model;
        if (!no_model) model = renderer.loadModel(vva::gfx::vulkan::ModelLoadInfo{
            .transform = transform,
            .mesh_path = (std::filesystem::path{VVA_ASSET_DIR}
                         / "models/viking_room.obj").string(),
            .texture_path = (std::filesystem::path{VVA_ASSET_DIR}
                            / "textures/viking_room.png").string()
        });

        using ProfileClock = std::chrono::steady_clock;
        using Section = vva::profile::Section;
        auto last_profile_report = ProfileClock::now();
        const auto warmup_start = last_profile_report;
        auto benchmark_start = warmup_start;
        bool measuring = false;
        vva::profile::Stats benchmark_stats;
        const auto accumulate = [&](const vva::profile::Stats& stats) {
            benchmark_stats.frames += stats.frames;
            for (std::size_t i = 0; i < stats.total_ms.size(); ++i)
                benchmark_stats.total_ms[i] += stats.total_ms[i];
        };
        vva_log_info("experiment: depth={}, model={}, validation={}, benchmark={} seconds (3s warmup)",
                     shared_depth ? "shared" : "per-frame", !no_model, validation, benchmark_seconds);
        while (!window.ShouldClose()) {
            {
                vva::profile::Scope frame_timer(Section::Frame);
                {
                    vva::profile::Scope timer(Section::Events);
                    window.PollEvents();
                    if (window.IsKeyPressed(vva::gfx::Key::Escape)) window.SetShouldClose(true);
                }

                fps_counter.update(window);
                renderer.startFrame();
                {
                    vva::profile::Scope timer(Section::Update);
                    if (model) renderer.getModelTransform(*model).rotate(
                        {vva::gfx::Vector3::UP}, 90.f * fps_counter.getDeltaTime());
                }
                if (model) renderer.drawModel(*model);
                renderer.endFrame();
            }
            vva::profile::cpu.endFrame();

            // Reporting is outside the frame timer; no per-frame logging.
            const auto now = ProfileClock::now();
            if (benchmark_seconds > 0 && !measuring && now - warmup_start >= std::chrono::seconds(3)) {
                (void)vva::profile::cpu.takeStats();
                benchmark_start = now;
                last_profile_report = now;
                measuring = true;
                vva_log_info("benchmark warmup complete");
            }
            if (benchmark_seconds > 0 && measuring &&
                now - benchmark_start >= std::chrono::seconds(benchmark_seconds)) {
                accumulate(vva::profile::cpu.takeStats());
                renderer.waitIdle();
                const auto elapsed = std::chrono::duration<double>(ProfileClock::now() - benchmark_start).count();
                vva_log_info("BENCHMARK depth={} model={} frames={} seconds={:.3f} fps={:.1f} "
                             "cpu={:.3f} fence={:.3f} acquire={:.3f} record={:.3f} submit={:.3f} present={:.3f}",
                             shared_depth ? "shared" : "per-frame", !no_model, benchmark_stats.frames,
                             elapsed, benchmark_stats.frames / elapsed,
                             benchmark_stats.msPerFrame(Section::Frame),
                             benchmark_stats.msPerFrame(Section::FenceWait),
                             benchmark_stats.msPerFrame(Section::Acquire),
                             benchmark_stats.msPerFrame(Section::Record),
                             benchmark_stats.msPerFrame(Section::Submit),
                             benchmark_stats.msPerFrame(Section::Present));
                break;
            }
            if (now - last_profile_report >= std::chrono::seconds(1)) {
                const auto stats = vva::profile::cpu.takeStats();
                if (measuring) accumulate(stats);
                vva_log_info(
                    "CPU ms/frame ({} frames): total={:.3f}, events={:.3f}, "
                    "fence={:.3f}, acquire={:.3f}, update={:.3f}, "
                    "record={:.3f}, submit={:.3f}, present={:.3f}",
                    stats.frames,
                    stats.msPerFrame(Section::Frame),
                    stats.msPerFrame(Section::Events),
                    stats.msPerFrame(Section::FenceWait),
                    stats.msPerFrame(Section::Acquire),
                    stats.msPerFrame(Section::Update),
                    stats.msPerFrame(Section::Record),
                    stats.msPerFrame(Section::Submit),
                    stats.msPerFrame(Section::Present));
                last_profile_report = now;
            }
        }
        vva_log_info("sandbox: renderer created, exiting");
        vva_log_info("vivian is going home!");
        vva::engine::log::shutdown();
        return 0;
    }
    catch (const std::exception& e) {
        vva_log_critical("fatal application error: {}", e.what());
        vva::engine::log::shutdown();
        return 1;
    }
}
