//
// Created by Wangsicong Wei on 2026-07-27.
//

#include "VulkanLogicalDeviceUtil.h"

#include "../VulkanDevice.h"
#include "../VulkanDeviceRequirement.h"
#include "../../../util/Log.h"

namespace Funccia::Graphic::Vulkan::Util {


    namespace {

        auto assembleVulkanDevice(
            vk::raii::PhysicalDevice physical_device,
                             const vk::DeviceCreateInfo& deviceCreateInfo,
                             const DeviceQueueCoordinates& queue_selection)->VulkanDevice {

            auto logical_device = vk::raii::Device(physical_device, deviceCreateInfo);
            vva_log_info("Logical Device Created");
            DeviceQueues queues {
                .graphics_queue_ = vk::raii::Queue(logical_device,
                                                   queue_selection.graphics.QueueFamilyIndex,
                                                   queue_selection.graphics.QueueIndex),
                .present_queue_ = vk::raii::Queue(logical_device,
                                                  queue_selection.present.QueueFamilyIndex,
                                                  queue_selection.present.QueueIndex),
                .compute_queue_ = vk::raii::Queue(logical_device,
                                                  queue_selection.compute.QueueFamilyIndex,
                                                  queue_selection.compute.QueueIndex),
                .transfer_queue_ = vk::raii::Queue(logical_device,
                                                   queue_selection.transfer.QueueFamilyIndex,
                                                   queue_selection.transfer.QueueIndex)
            };
            vva_log_info("Vulkan Device Created");
            return VulkanDevice {
                .physical_device = std::move(physical_device),
                .logical_device = std::move(logical_device),
                .queues = std::move(queues),
                .queue_coordinates = queue_selection
            };
        }

        auto SelectAllCapableQueueFamily(const vk::raii::SurfaceKHR& surface,
                                         const vk::raii::PhysicalDevice& physical_device) -> std::optional<QueueCoordinate> {
            std::vector<vk::QueueFamilyProperties2> queue_family_properties = physical_device.
                getQueueFamilyProperties2();

            constexpr auto required = VulkanDeviceRequirement::required_queue_flags;

            for (size_t i = 0; i < queue_family_properties.size(); i++) {
                if (((queue_family_properties[i].queueFamilyProperties.queueFlags & required) == required)
                    && (physical_device.getSurfaceSupportKHR(static_cast<uint32_t>(i), *surface))) {
                    vva_log_info("Found Queue Family Capable of Graphics, Compute, Transfer, Present at index {}", i);
                    return std::optional<QueueCoordinate>({static_cast<uint32_t>(i), 0});
                }
            }
            return std::nullopt;
        }

    }
    auto SelectDeviceQueues(const vk::raii::SurfaceKHR& surface, const vk::raii::PhysicalDevice& physical_device) -> std::optional<DeviceQueueCoordinates> {
        //std::vector<vk::QueueFamilyProperties> queueFamilyProperties = device.physical_device_.getQueueFamilyProperties2();
        if (const auto result = SelectAllCapableQueueFamily(surface, physical_device)) {
            const QueueCoordinate graphics = result.value();
            return std::optional<DeviceQueueCoordinates>{{.graphics = graphics, .present = graphics, .compute = graphics, .transfer = graphics}};
        }
        else {
            vva_log_error("Suitable Device not found");
            return std::nullopt;
        }
    }
    auto CreateVulkanDevice(
        vk::raii::PhysicalDevice physicalDevice, DeviceQueueCoordinates queue_selection) -> VulkanDevice {

        float queue_priority = 0.5;
        vk::DeviceQueueCreateInfo device_queue_create_info{
            .queueFamilyIndex = queue_selection.graphics.QueueFamilyIndex,
            .queueCount = 1,
            .pQueuePriorities = &queue_priority
        };

        // Create a chain of feature structures

        auto required_features = VulkanDeviceRequirement::features();

        vk::DeviceCreateInfo deviceCreateInfo{
            .pNext = &required_features.get<vk::PhysicalDeviceFeatures2>(),
            .queueCreateInfoCount = 1,
            .pQueueCreateInfos = &device_queue_create_info,
            .enabledExtensionCount = static_cast<uint32_t>(VulkanDeviceRequirement::extensions.size()),
            .ppEnabledExtensionNames = VulkanDeviceRequirement::extensions.data()
        };

        return assembleVulkanDevice(std::move(physicalDevice),
            deviceCreateInfo,
            queue_selection);


    }

    uint32_t findTransferQue(const uint32_t graphicsIndex,
                             const vk::raii::PhysicalDevice& physicalDevice) {
        std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice.getQueueFamilyProperties();
        //find if there's any queue that is specialized for transfer
        auto transferQueueFamilyProperty = std::ranges::find_if(queueFamilyProperties, [](auto const& qfp) {
            return (qfp.queueFlags & vk::QueueFlagBits::eTransfer) != static_cast<vk::QueueFlags>(0)
                && (qfp.queueFlags & vk::QueueFlagBits::eGraphics) == static_cast<vk::QueueFlags>(0)
                && (qfp.queueFlags & vk::QueueFlagBits::eCompute) == static_cast<vk::QueueFlags>(0);
        });
        auto transferIndex = static_cast<uint32_t>(std::distance(queueFamilyProperties.begin(),
                                                                 transferQueueFamilyProperty));

        if (transferIndex == queueFamilyProperties.size()) {
            //check if the current graphics queue can do transfer
            transferIndex = (queueFamilyProperties[graphicsIndex].queueFlags & vk::QueueFlagBits::eTransfer) !=
                            static_cast<vk::QueueFlags>(0)
                                ? graphicsIndex
                                : static_cast<uint32_t>(queueFamilyProperties.size());
            if (transferIndex == queueFamilyProperties.size()) {
                //if not, find if there's any queue that can do transfer
                transferQueueFamilyProperty = std::ranges::find_if(queueFamilyProperties, [](auto const& qfp) {
                    return (qfp.queueFlags & vk::QueueFlagBits::eTransfer) != static_cast<vk::QueueFlags>(0);
                });
                transferIndex = static_cast<uint32_t>(std::distance(queueFamilyProperties.begin(),
                                                                    transferQueueFamilyProperty));
                if (transferIndex == queueFamilyProperties.size()) {
                    throw std::runtime_error("failed to find a transfer queue family!");
                }
            }
        }
        return transferIndex;
    }
}
