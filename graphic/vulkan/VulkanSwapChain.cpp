//
// Created by Wangsicong Wei on 2026-08-27.
//

#include "VulkanSwapChain.h"

#include "../../util/Log.h"


namespace Funccia::Graphic::Vulkan {
    namespace {
        auto chooseSwapSurfaceFormat(
            const std::vector<vk::SurfaceFormatKHR>& availableFormats) -> vk::SurfaceFormatKHR {
            if (availableFormats.empty()) {
                throw std::runtime_error("surface does not support any swapchain formats");
            }
            for (const auto& availableFormat : availableFormats) {
                if (availableFormat.format == vk::Format::eB8G8R8A8Srgb && availableFormat.colorSpace ==
                    vk::ColorSpaceKHR::eSrgbNonlinear) {
                    vva_log_info("swapchain surface format set, format: eB8G8R8A8Srgb, color space: eSrgbNonlinear");
                    return availableFormat;
                }
            }
            vva_log_debug("BGRA format not found supported by surface, finding RGBA instead");
            for (const auto& availableFormat : availableFormats) {
                if (availableFormat.format == vk::Format::eR8G8B8A8Srgb && availableFormat.colorSpace ==
                    vk::ColorSpaceKHR::eSrgbNonlinear) {
                    vva_log_info("swapchain surface format set, format: eR8G8B8A8Srgb, color space: eSrgbNonlinear");
                    return availableFormat;
                }
            }
            vva_log_debug("RGBA format not found supported by surface, fallback to the first supported format");
            return availableFormats.front();
        }

        auto chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes) -> vk::PresentModeKHR {
            if (availablePresentModes.empty()) {
                throw std::runtime_error("available present modes empty");
            }
            for (const auto& availablePresentMode : availablePresentModes) {
                if (availablePresentMode == vk::PresentModeKHR::eMailbox) {
                    return availablePresentMode;
                }
            }
            return vk::PresentModeKHR::eFifo;
        }

        auto chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities,
                              const glm::ivec2 buffer_size) -> vk::Extent2D {
            //if the current extent is max uint32 value, then the application
            //must choose a value. Otherwise the app should use the current extent set by vulkan
            if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
                vva_log_info("swapchain extent chosen by vulkan surface: width: {}, height: {}",
                             capabilities.currentExtent.width, capabilities.currentExtent.height);
                return capabilities.currentExtent;
            }
            if (buffer_size.x <= 0 || buffer_size.y <= 0) {
                vva_log_error("buffer size can not be negative");
                return {};
            }

            const auto width = static_cast<uint32_t>(buffer_size.x);
            const auto height = static_cast<uint32_t>(buffer_size.y);
            vva_log_info("swapchain extent chosen by application: width: {}, height: {}", width, height);
            return {
                .width = std::clamp<uint32_t>(width, capabilities.minImageExtent.width,
                                              capabilities.maxImageExtent.width),
                .height = std::clamp<uint32_t>(height, capabilities.minImageExtent.height,
                                               capabilities.maxImageExtent.height)
            };
        }

        uint32_t chooseSwapMinImageCount(vk::SurfaceCapabilitiesKHR const& surfaceCapabilities) {
            // TODO: move the hardcoded preferred image count out somewhere
            auto minImageCount = std::max(3u, surfaceCapabilities.minImageCount);
            if ((0 < surfaceCapabilities.maxImageCount) && (surfaceCapabilities.maxImageCount < minImageCount)) {
                minImageCount = surfaceCapabilities.maxImageCount;
            }
            vva_log_info("swapchain image count: {}", minImageCount);
            return minImageCount;
        }

        auto createSwapChainImageViews(const std::vector<vk::Image>& swapChainImages, const vk::Format format,
                                       const vk::raii::Device& device) -> std::vector<VulkanSwapChainImage> {
            vk::ImageViewCreateInfo imageViewCreateInfo{
                .viewType = vk::ImageViewType::e2D,
                .format = format,
                .subresourceRange = {vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1}
            };
            std::vector<VulkanSwapChainImage> vulkan_swap_chain_images;
            for (auto& image : swapChainImages) {
                imageViewCreateInfo.image = image;
                vulkan_swap_chain_images.emplace_back(
                    VulkanSwapChainImage{
                        .image = image,
                        .image_view = {device, imageViewCreateInfo}
                    });
            }
            return vulkan_swap_chain_images;
        }
    }

    auto VulkanSwapChain::CreateSwapChain(const vk::raii::SurfaceKHR& surface,
                                          const vk::raii::PhysicalDevice& physical_device,
                                          const vk::raii::Device& device,
                                          const glm::ivec2 buffer_size) -> VulkanSwapChain {
        const auto surface_capabilities = physical_device.getSurfaceCapabilitiesKHR(*surface);
        const auto swap_chain_extent = chooseSwapExtent(surface_capabilities, buffer_size);
        if (swap_chain_extent.height == 0 || swap_chain_extent.width == 0) {
            vva_log_debug("caution: swap chain extent height or width is 0");
        }
        const auto swap_chain_surface_format = chooseSwapSurfaceFormat(
            physical_device.getSurfaceFormatsKHR(*surface));
        // MARK: add or change .queueFamilyIndexCount = 0
        // .pQueueFamilyIndices = nullptr
        // when there's more than 1 queue family
        // TODO: extract swap chain creation settings to a central place rather than placing then in the file
        vk::SwapchainCreateInfoKHR swap_chain_create_info{
            .surface = *surface,
            .minImageCount = chooseSwapMinImageCount(surface_capabilities),
            .imageFormat = swap_chain_surface_format.format,
            .imageColorSpace = swap_chain_surface_format.colorSpace,
            .imageExtent = swap_chain_extent,
            .imageArrayLayers = 1,
            .imageUsage = vk::ImageUsageFlagBits::eColorAttachment,
            .imageSharingMode = vk::SharingMode::eExclusive,
            .preTransform = surface_capabilities.currentTransform,
            .compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque,
            .presentMode = chooseSwapPresentMode(physical_device.getSurfacePresentModesKHR(*surface)),
            .clipped = true
        };

        auto swapChain = vk::raii::SwapchainKHR(device, swap_chain_create_info);
        auto swapChainImages = createSwapChainImageViews(swapChain.getImages(), swap_chain_surface_format.format, device);

        return VulkanSwapChain{
            .handle = std::move(swapChain),
            .images = std::move(swapChainImages),
            .surface_format = swap_chain_surface_format,
            .extent = swap_chain_extent
        };
    }
}
