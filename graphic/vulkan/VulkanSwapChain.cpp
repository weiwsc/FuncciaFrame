//
// Created by Wangsicong Wei on 2026-08-27.
//

#include "VulkanSwapChain.h"



namespace Funccia::Graphic::Vulkan {
    namespace {
        vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats) {
            for (const auto& availableFormat : availableFormats) {
                if (availableFormat.format == vk::Format::eB8G8R8A8Srgb && availableFormat.colorSpace ==
                    vk::ColorSpaceKHR::eSrgbNonlinear) {
                    return availableFormat;
                }
            }

            return availableFormats[0];
        }

        vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes) {
            for (const auto& availablePresentMode : availablePresentModes) {
                if (availablePresentMode == vk::PresentModeKHR::eMailbox) {
                    return availablePresentMode;
                }
            }
            return vk::PresentModeKHR::eFifo;
        }

        vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities, const glm::ivec2 buffer_size) {
            if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
                return capabilities.currentExtent;
            }
            const int width = buffer_size.x;
            const int height = buffer_size.y;

            return {
                std::clamp<uint32_t>(width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width),
                std::clamp<uint32_t>(height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height)
            };
        }
        uint32_t chooseSwapMinImageCount(vk::SurfaceCapabilitiesKHR const &surfaceCapabilities) {
            auto minImageCount = std::max(3u, surfaceCapabilities.minImageCount);
            if ((0 < surfaceCapabilities.maxImageCount) && (surfaceCapabilities.maxImageCount < minImageCount)) {
                minImageCount = surfaceCapabilities.maxImageCount;
            }
            return minImageCount;
        }
    }

    auto VulkanSwapChain::CreateSwapChian(const vk::raii::SurfaceKHR& surface,
                                          const vk::raii::PhysicalDevice& physical_device,
                                          const vk::raii::Device& device,
                                          const glm::ivec2 buffer_size) -> VulkanSwapChain{
        auto surfaceCapabilities = physical_device.getSurfaceCapabilitiesKHR(*surface);
        auto swapChainExtent = chooseSwapExtent(surfaceCapabilities, buffer_size);
        auto swapChainSurfaceFormat = chooseSwapSurfaceFormat(physical_device.getSurfaceFormatsKHR(*surface));
        vk::SwapchainCreateInfoKHR swapChainCreateInfo{
            .surface = *surface,
            .minImageCount = chooseSwapMinImageCount(surfaceCapabilities),
            .imageFormat = swapChainSurfaceFormat.format,
            .imageColorSpace = swapChainSurfaceFormat.colorSpace,
            .imageExtent = swapChainExtent,
            .imageArrayLayers = 1,
            .imageUsage = vk::ImageUsageFlagBits::eColorAttachment,
            .imageSharingMode = vk::SharingMode::eExclusive,
            .preTransform = surfaceCapabilities.currentTransform,
            .compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque,
            .presentMode = chooseSwapPresentMode(physical_device.getSurfacePresentModesKHR(*surface)),
            .clipped = true
        };

        auto swapChain = vk::raii::SwapchainKHR(device, swapChainCreateInfo);
        auto swapChainImages = swapChain.getImages();
        return VulkanSwapChain {
            .handle = std::move(swapChain),
            .images = std::move(swapChainImages),
            .surface_format = swapChainSurfaceFormat,
            .extent = swapChainExtent
        };
    }
}
