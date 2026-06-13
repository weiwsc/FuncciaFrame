//
// Created by Wangsicong Wei on 2026-06-13.
//

#pragma once
#include <iostream>
#include <span>
#include "vulkan/vulkan_raii.hpp"

namespace Funccia::Graphic::Vulkan {
    class VulkanPhysicalDevice {
    public:
        [[nodiscard]]
        static auto pickPhysicalDevice(const vk::raii::Instance& instance,
                                       std::span<char const* const> requiredDeviceExtensions)
            -> vk::raii::PhysicalDevice {
            auto devices = instance.enumeratePhysicalDevices();
            const auto devIter = std::ranges::find_if(devices, [&](auto const& device) {
                std::cout<< "Physical Device: " << device.getProperties2().properties.deviceName << std::endl;
                return isSuitable(device, requiredDeviceExtensions);
            });
            if (devIter == devices.end()) {
                throw std::runtime_error("failed to find a suitable GPU!");
            }
            return *devIter;
        }

    private:
        static auto hasGraphicsQueue(const vk::raii::PhysicalDevice& dev) -> bool {
            auto qfs = dev.getQueueFamilyProperties();
            return std::ranges::any_of(qfs, [](auto const& q) {
                return static_cast<bool>(q.queueFlags & vk::QueueFlagBits::eGraphics);
            });
        }

        static auto supportsExtensions(const vk::raii::PhysicalDevice& dev,
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

        static auto isSuitable(const vk::raii::PhysicalDevice& dev,
                               std::span<char const* const> required) -> bool {
            return dev.getProperties().apiVersion >= VK_API_VERSION_1_3
                && dev.getFeatures2().features.samplerAnisotropy == vk::True
                && hasGraphicsQueue(dev)
                && supportsExtensions(dev, required);
        }
    };
}
