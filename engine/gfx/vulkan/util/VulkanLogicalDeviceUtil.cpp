//
// Created by Wangsicong Wei on 2026-07-27.
//

#include "VulkanLogicalDeviceUtil.h"

#include "../VulkanDevice.h"
#include "../VulkanConfig.h"
#include "core/Log.h"

namespace vva::gfx::vulkan::util {


    namespace {

        auto assembleVulkanDevice(
            vk::raii::PhysicalDevice physical_device,
                             const vk::DeviceCreateInfo& device_create_info,
                             const DeviceQueueCoordinates& queue_selection)->VulkanDevice {

            auto logical_device = vk::raii::Device(physical_device, device_create_info);
            vva_log_info("Logical Device Created");
            DeviceQueues queues {
                .graphics_queue = vk::raii::Queue(logical_device,
                                                   queue_selection.graphics.queue_family_index,
                                                   queue_selection.graphics.queue_index),
                .present_queue = vk::raii::Queue(logical_device,
                                                  queue_selection.present.queue_family_index,
                                                  queue_selection.present.queue_index),
                .compute_queue = vk::raii::Queue(logical_device,
                                                  queue_selection.compute.queue_family_index,
                                                  queue_selection.compute.queue_index),
                .transfer_queue = vk::raii::Queue(logical_device,
                                                   queue_selection.transfer.queue_family_index,
                                                   queue_selection.transfer.queue_index)
            };

            vva_log_info("Vulkan Device Created");
            return VulkanDevice {
                .physical_device = std::move(physical_device),
                .logical_device = std::move(logical_device),
                .queues = std::move(queues),
                .queue_coordinates = queue_selection
            };
        }

        auto selectAllCapableQueueFamily(const vk::raii::SurfaceKHR& surface,
                                         const vk::raii::PhysicalDevice& physical_device) -> std::optional<QueueCoordinate> {
            std::vector<vk::QueueFamilyProperties2> queue_family_properties = physical_device.
                getQueueFamilyProperties2();

            constexpr auto kRequired = VulkanDeviceRequirement::required_queue_flags;

            for (size_t i = 0; i < queue_family_properties.size(); i++) {
                if (((queue_family_properties[i].queueFamilyProperties.queueFlags & kRequired) == kRequired)
                    && (physical_device.getSurfaceSupportKHR(static_cast<uint32_t>(i), *surface))) {
                    vva_log_info("Found Queue Family Capable of Graphics, Compute, Transfer, Present at index {}", i);
                    return std::optional<QueueCoordinate>({static_cast<uint32_t>(i), 0});
                }
            }
            return std::nullopt;
        }

    }
    auto selectDeviceQueues(const vk::raii::SurfaceKHR& surface, const vk::raii::PhysicalDevice& physical_device) -> std::optional<DeviceQueueCoordinates> {
        //std::vector<vk::QueueFamilyProperties> queueFamilyProperties = device.physical_device_.getQueueFamilyProperties2();
        if (const auto result = selectAllCapableQueueFamily(surface, physical_device)) {
            const QueueCoordinate graphics = result.value();
            return std::optional<DeviceQueueCoordinates>{{.graphics = graphics, .present = graphics, .compute = graphics, .transfer = graphics}};
        }
        else {
            vva_log_error("Suitable Device not found");
            return std::nullopt;
        }
    }
    auto createVulkanDevice(
        vk::raii::PhysicalDevice physical_device, DeviceQueueCoordinates queue_selection) -> VulkanDevice {

        float queue_priority = 0.5;
        vk::DeviceQueueCreateInfo device_queue_create_info{
            .queueFamilyIndex = queue_selection.graphics.queue_family_index,
            .queueCount = 1,
            .pQueuePriorities = &queue_priority
        };

        // Create a chain of feature structures

        auto required_features = VulkanDeviceRequirement::features();

        vk::DeviceCreateInfo device_create_info{
            .pNext = &required_features.get<vk::PhysicalDeviceFeatures2>(),
            .queueCreateInfoCount = 1,
            .pQueueCreateInfos = &device_queue_create_info,
            .enabledExtensionCount = static_cast<uint32_t>(VulkanDeviceRequirement::extensions.size()),
            .ppEnabledExtensionNames = VulkanDeviceRequirement::extensions.data()
        };

        return assembleVulkanDevice(std::move(physical_device),
            device_create_info,
            queue_selection);


    }

    uint32_t findTransferQue(const uint32_t graphics_index,
                             const vk::raii::PhysicalDevice& physical_device) {
        std::vector<vk::QueueFamilyProperties> queue_family_properties = physical_device.getQueueFamilyProperties();
        //find if there's any queue that is specialized for transfer
        auto transfer_queue_family_property = std::ranges::find_if(queue_family_properties, [](auto const& qfp) {
            return (qfp.queueFlags & vk::QueueFlagBits::eTransfer) != static_cast<vk::QueueFlags>(0)
                && (qfp.queueFlags & vk::QueueFlagBits::eGraphics) == static_cast<vk::QueueFlags>(0)
                && (qfp.queueFlags & vk::QueueFlagBits::eCompute) == static_cast<vk::QueueFlags>(0);
        });
        auto transfer_index = static_cast<uint32_t>(std::distance(queue_family_properties.begin(),
                                                                 transfer_queue_family_property));

        if (transfer_index == queue_family_properties.size()) {
            //check if the current graphics queue can do transfer
            transfer_index = (queue_family_properties[graphics_index].queueFlags & vk::QueueFlagBits::eTransfer) !=
                            static_cast<vk::QueueFlags>(0)
                                ? graphics_index
                                : static_cast<uint32_t>(queue_family_properties.size());
            if (transfer_index == queue_family_properties.size()) {
                //if not, find if there's any queue that can do transfer
                transfer_queue_family_property = std::ranges::find_if(queue_family_properties, [](auto const& qfp) {
                    return (qfp.queueFlags & vk::QueueFlagBits::eTransfer) != static_cast<vk::QueueFlags>(0);
                });
                transfer_index = static_cast<uint32_t>(std::distance(queue_family_properties.begin(),
                                                                    transfer_queue_family_property));
                if (transfer_index == queue_family_properties.size()) {
                    throw std::runtime_error("failed to find a transfer queue family!");
                }
            }
        }
        return transfer_index;
    }
}
