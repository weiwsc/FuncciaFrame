//
// Created by Wangsicong Wei on 2026-07-27.
//
#include <iostream>
#include "VulkanPhysicalDeviceUtil.h"

#include "VulkanLogicalDeviceUtil.h"
#include "../VulkanConfig.h"
#include "core/Log.h"
#include "../types/VulkanTypesDef.h"

namespace vva::gfx::vulkan::util {
    namespace {
        // auto hasGraphicsQueue(const vk::raii::PhysicalDevice& dev) -> bool {
        //     auto qfs = dev.getQueueFamilyProperties();
        //     return std::ranges::any_of(qfs, [](auto const& q) {
        //         return static_cast<bool>(q.queueFlags & vk::QueueFlagBits::eGraphics);
        //     });
        // }

        auto supportsExtensions(const vk::raii::PhysicalDevice& dev,
                                std::span<char const* const> required) -> bool {
            auto avail = dev.enumerateDeviceExtensionProperties();
            bool ok = true;
            for (auto const* req : required) {
                bool found = std::ranges::any_of(avail, [req](auto const& e) {
                    return strcmp(e.extensionName, req) == 0;
                });
                if (!found) std::cerr << "device missing extension: " << req << "\n";
                ok = ok && found;
            }
            return ok;
        }
        auto isSwapChainAvailable(const vk::raii::PhysicalDevice& device, const vk::raii::SurfaceKHR& surface) -> bool {
            return !device.getSurfaceFormatsKHR(*surface).empty() &&
                    !device.getSurfacePresentModesKHR(*surface).empty();
        }
        auto isSuitable(const vk::raii::PhysicalDevice& device, const vk::raii::SurfaceKHR& surface ) -> bool {
            return device.getProperties().apiVersion >=
           VulkanDeviceRequirement::MINIMUM_API_VERSION
    && VulkanDeviceRequirement::isRequiredFeatureSupported(device)
    && supportsExtensions(device, VulkanDeviceRequirement::EXTENSIONS)
    && isSwapChainAvailable(device, surface);
        }
    }

    [[nodiscard]]
    auto pickPhysicalDevice(const vk::raii::Instance& instance, const vk::raii::SurfaceKHR& surface)
        -> PickPhysicalDeviceResult {
        auto devices = instance.enumeratePhysicalDevices();
        DeviceQueueCoordinates queue_selection {};
        const auto dev_iter = std::ranges::find_if(devices, [&](auto const& device) {
            vva_log_info("Physical Device: {}", device.getProperties2().properties.deviceName.data());
            //check if the device has the required queue family
            if (const auto selection_result= util::selectDeviceQueues(surface, device)) {
                queue_selection = selection_result.value();
            } else {return false;}
            return isSuitable(device, surface);
        });
        if (dev_iter == devices.end()) {
            vva_log_error("failed to find a suitable GPU!");
            throw std::runtime_error("failed to find a suitable GPU!");
        }
        return PickPhysicalDeviceResult {
            .physical_device = *dev_iter,
            .queue_coordinates = queue_selection
        };
    }
}
