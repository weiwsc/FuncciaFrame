//
// Created by Wangsicong Wei on 2026-08-29.
//

#include "SlangShaderCompiler.h"

#include <array>

#include "../../util/Log.h"

namespace vva::gfx::shader {
    namespace {
        void report(Slang::ComPtr<slang::IBlob>& d) {
            if (d) { std::fprintf(stderr, "%s\n", (const char*)d->getBufferPointer()); d = nullptr; }
        }
    }
    auto SlangShaderCompiler::create() -> SlangShaderCompiler {
        Slang::ComPtr<slang::IGlobalSession> global_session;
        slang::createGlobalSession(global_session.writeRef());

        slang::SessionDesc session_desc = {};
        //TODO: separate slang global session and session?
        slang::TargetDesc target_desc = {};
        target_desc.format = SLANG_SPIRV;
        target_desc.profile = global_session->findProfile("spirv_1_6");
        //https://docs.shader-slang.org/en/latest/external/slang/docs/user-guide/08-compiling.html#pre-defined-macros
        // std::array<slang::PreprocessorMacroDesc, 2> preprocessorMacroDesc =
        // {
        //     slang::PreprocessorMacroDesc{ .name = "BIAS_VALUE", .value = "1138" },
        //     slang::PreprocessorMacroDesc{ .name = "OTHER_MACRO", .value = "float" }
        // };
        // session_desc.preprocessorMacros = preprocessorMacroDesc.data();
        // session_desc.preprocessorMacroCount = preprocessorMacroDesc.size();

        //https://docs.shader-slang.org/en/latest/external/slang/docs/user-guide/08-compiling.html#compiler-options
        std::array options{
            slang::CompilerOptionEntry{
                slang::CompilerOptionName::EmitSpirvDirectly,
                {slang::CompilerOptionValueKind::Int, 1}
            },
        };

        session_desc.targets = &target_desc;
        session_desc.targetCount = 1;
        session_desc.defaultMatrixLayoutMode = SLANG_MATRIX_LAYOUT_COLUMN_MAJOR;
        session_desc.compilerOptionEntries = options.data();
        session_desc.compilerOptionEntryCount = static_cast<uint32_t>(options.size());
        Slang::ComPtr<slang::ISession> session;
        global_session->createSession(session_desc, session.writeRef());
        vva_log_info("slang shader compiler created");
        return SlangShaderCompiler {
            std::move(global_session),
            std::move(session)
        };
    }

    auto SlangShaderCompiler::compile(const std::string& module_name,
        const std::string& entry_point) const -> std::vector<uint32_t> {

        //https://shader-slang.org/docs/compilation-api/
        Slang::ComPtr<slang::IBlob> diag;

        slang::IModule* module = session_->loadModule(module_name.data(), diag.writeRef());
        report(diag);
        if (!module) throw std::runtime_error("slang: failed to load module");

        Slang::ComPtr<slang::IEntryPoint> ep;
        module->findEntryPointByName(entry_point.data(), ep.writeRef());
        if (!ep) throw std::runtime_error("slang: entry point not found");

        std::array<slang::IComponentType*, 2> parts{ module, ep };
        Slang::ComPtr<slang::IComponentType> composed, linked;
        session_->createCompositeComponentType(parts.data(), parts.size(),
                                               composed.writeRef(), diag.writeRef());
        report(diag);
        composed->link(linked.writeRef(), diag.writeRef());
        report(diag);

        Slang::ComPtr<slang::IBlob> code;
        linked->getEntryPointCode(/*entryPointIndex*/0, /*targetIndex*/0,
                                  code.writeRef(), diag.writeRef());
        report(diag);
        if (!code) throw std::runtime_error("slang: codegen failed");

        auto* p = static_cast<const uint32_t*>(code->getBufferPointer());
        return { p, p + code->getBufferSize() / sizeof(uint32_t) };
    }
}
