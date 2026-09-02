//
// Created by Wangsicong Wei on 2026-08-31.
//
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "Texture.h"


#include <cstring>

#include "../AllocatedBuffer.h"
#include "../VulkanDevice.h"
#include "../VulkanUploadContext.h"

namespace vva::gfx::vulkan{
    namespace {
        struct DecodedImage {
            uint32_t width;
            uint32_t height;
            std::vector<std::byte> rgba;
        };

        auto decodeImageRgba(const std::filesystem::path& path) -> DecodedImage {
            using SurfacePtr =
                std::unique_ptr<SDL_Surface, decltype(&SDL_DestroySurface)>;

            SurfacePtr loaded{
                IMG_Load(path.string().c_str()),
                SDL_DestroySurface
            };

            if (!loaded) {
                throw std::runtime_error(
                    "IMG_Load failed: " + std::string(SDL_GetError()));
            }

            SurfacePtr converted{
                SDL_ConvertSurface(loaded.get(), SDL_PIXELFORMAT_RGBA32),
                SDL_DestroySurface
            };

            if (!converted) {
                throw std::runtime_error(
                    "SDL_ConvertSurface failed: " +
                    std::string(SDL_GetError()));
            }

            if (converted->w <= 0 || converted->h <= 0) {
                throw std::runtime_error("decoded image has invalid dimensions");
            }

            const auto width = static_cast<uint32_t>(converted->w);
            const auto height = static_cast<uint32_t>(converted->h);
            constexpr std::size_t kBytesPerPixel = 4;

            DecodedImage result{
                .width = width,
                .height = height,
                .rgba = std::vector<std::byte>(
                    static_cast<std::size_t>(width) * height * kBytesPerPixel)
            };

            if (!SDL_LockSurface(converted.get())) {
                throw std::runtime_error(
                    "SDL_LockSurface failed: " +
                    std::string(SDL_GetError()));
            }

            const auto* source =
                static_cast<const std::byte*>(converted->pixels);

            const auto destination_pitch =
                static_cast<std::size_t>(width) * kBytesPerPixel;

            // Do not assume SDL_Surface::pitch == width * 4.
            for (uint32_t y = 0; y < height; ++y) {
                std::memcpy(
                    result.rgba.data() + y * destination_pitch,
                    source + y * converted->pitch,
                    destination_pitch);
            }

            SDL_UnlockSurface(converted.get());
            return result;
        }
    }

    auto createTextureImage(
        VmaAllocator allocator,
        const vk::raii::Device& device,
        const vk::ImageCreateInfo& image_create_info,
        vk::Format view_format, vk::ImageAspectFlags view_aspect_flags)
        -> Texture2D {
        auto image = AllocatedImage::create(image_create_info, allocator);

        vk::ImageViewCreateInfo image_view_create_info{
            .image = image.handle(),
            .viewType = vk::ImageViewType::e2D,
            .format = view_format,
            .subresourceRange = {
                .aspectMask = view_aspect_flags,
                .baseMipLevel = 0,
                .levelCount = 1,
                .baseArrayLayer = 0,
                .layerCount = 1
            }
        };

        vk::raii::ImageView view{device, image_view_create_info};

        return Texture2D{
            .source_image = std::move(image),
            .image_view = std::move(view),
            .extent = {image_create_info.extent.width, image_create_info.extent.height},
            .format = image_create_info.format
        };
    }

    auto loadTextureFromFile(
        VmaAllocator allocator,
        const VulkanDevice& device,
        VulkanUploadContext& upload,
        const std::filesystem::path& path) -> Texture2D {
        const DecodedImage decoded = decodeImageRgba(path);
        const vk::Extent2D extent{decoded.width, decoded.height};

        // Host-visible staging buffer, persistently mapped
        auto staging = AllocatedBuffer::createBuffer(
            allocator,
            decoded.rgba.size(),
            vk::BufferUsageFlagBits::eTransferSrc,
            VMA_MEMORY_USAGE_AUTO_PREFER_HOST,
            VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
            VMA_ALLOCATION_CREATE_MAPPED_BIT);

        std::memcpy(staging.mappedData(), decoded.rgba.data(), decoded.rgba.size());
        vmaFlushAllocation(allocator, staging.allocation(), 0, VK_WHOLE_SIZE);

        vk::ImageCreateInfo image_info{
            .imageType = vk::ImageType::e2D,
            .format = vk::Format::eR8G8B8A8Srgb,
            .extent = {
                .width = extent.width,
                .height = extent.height,
                .depth = 1
            },
            .mipLevels = 1,
            .arrayLayers = 1,
            .samples = vk::SampleCountFlagBits::e1,
            .tiling = vk::ImageTiling::eOptimal,
            .usage = vk::ImageUsageFlagBits::eTransferDst |
            vk::ImageUsageFlagBits::eSampled,
            .sharingMode = vk::SharingMode::eExclusive,
            .initialLayout = vk::ImageLayout::eUndefined
        };

        Texture2D texture = createTextureImage(
            allocator, device.logical_device, image_info, image_info.format, vk::ImageAspectFlagBits::eColor);

        const auto& cmd = upload.command_buffer;
        device.logical_device.resetFences(*upload.fence);
        cmd.reset();
        cmd.begin({.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit});

        constexpr vk::ImageSubresourceRange kColorRange{
            .aspectMask = vk::ImageAspectFlagBits::eColor,
            .baseMipLevel = 0, .levelCount = 1,
            .baseArrayLayer = 0, .layerCount = 1
        };

        vk::ImageMemoryBarrier2 to_transfer_dst{
            .srcStageMask = vk::PipelineStageFlagBits2::eTopOfPipe,
            .dstStageMask = vk::PipelineStageFlagBits2::eCopy,
            .dstAccessMask = vk::AccessFlagBits2::eTransferWrite,
            .oldLayout = vk::ImageLayout::eUndefined,
            .newLayout = vk::ImageLayout::eTransferDstOptimal,
            .srcQueueFamilyIndex = vk::QueueFamilyIgnored,
            .dstQueueFamilyIndex = vk::QueueFamilyIgnored,
            .image = texture.source_image.handle(),
            .subresourceRange = kColorRange
        };
        cmd.pipelineBarrier2({
            .imageMemoryBarrierCount = 1,
            .pImageMemoryBarriers = &to_transfer_dst
        });

        const vk::BufferImageCopy region{
            .bufferOffset = 0,
            .bufferRowLength = 0, // 0 = tightly packed, which DecodeImageRGBA guarantees
            .bufferImageHeight = 0,
            .imageSubresource = {
                .aspectMask = vk::ImageAspectFlagBits::eColor,
                .mipLevel = 0, .baseArrayLayer = 0, .layerCount = 1
            },
            .imageOffset = {0, 0, 0},
            .imageExtent = {extent.width, extent.height, 1}
        };
        cmd.copyBufferToImage(
            staging.handle(),
            texture.source_image.handle(),
            vk::ImageLayout::eTransferDstOptimal,
            region);

        vk::ImageMemoryBarrier2 to_shader_read{
            .srcStageMask = vk::PipelineStageFlagBits2::eCopy,
            .srcAccessMask = vk::AccessFlagBits2::eTransferWrite,
            .dstStageMask = vk::PipelineStageFlagBits2::eFragmentShader,
            .dstAccessMask = vk::AccessFlagBits2::eShaderSampledRead,
            .oldLayout = vk::ImageLayout::eTransferDstOptimal,
            .newLayout = vk::ImageLayout::eShaderReadOnlyOptimal,
            .srcQueueFamilyIndex = vk::QueueFamilyIgnored,
            .dstQueueFamilyIndex = vk::QueueFamilyIgnored,
            .image = texture.source_image.handle(),
            .subresourceRange = kColorRange
        };
        cmd.pipelineBarrier2({
            .imageMemoryBarrierCount = 1,
            .pImageMemoryBarriers = &to_shader_read
        });

        cmd.end();

        const vk::SubmitInfo submit{
            .commandBufferCount = 1,
            .pCommandBuffers = &*cmd
        };
        device.queues.graphics_queue.submit(submit, *upload.fence);
        (void)device.logical_device.waitForFences(*upload.fence, vk::True, UINT64_MAX);

        return texture; // staging buffer is freed here by RAII, safely after the wait
    }

    auto createTextureSampler(const vk::raii::Device& device,
                              const vk::raii::PhysicalDevice& physical_device) -> vk::raii::Sampler {
        vk::PhysicalDeviceProperties properties = physical_device.getProperties();
        vk::SamplerCreateInfo sampler_info{
            .magFilter = vk::Filter::eLinear,
            .minFilter = vk::Filter::eLinear,
            .mipmapMode = vk::SamplerMipmapMode::eLinear,
            .addressModeU = vk::SamplerAddressMode::eRepeat,
            .addressModeV = vk::SamplerAddressMode::eRepeat,
            .addressModeW = vk::SamplerAddressMode::eRepeat,
            .mipLodBias = 0.0f,
            .anisotropyEnable = vk::True,
            .maxAnisotropy = properties.limits.maxSamplerAnisotropy,
            .compareEnable = vk::False,
            .compareOp = vk::CompareOp::eAlways
        };
        return vk::raii::Sampler(device, sampler_info);
    }
}
