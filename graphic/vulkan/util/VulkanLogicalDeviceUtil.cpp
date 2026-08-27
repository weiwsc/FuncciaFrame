//
// Created by Wangsicong Wei on 2026-07-27.
//

#include "VulkanLogicalDeviceUtil.h"

#include "../VulkanDevice.h"
#include "../../../util/Log.h"

namespace Funccia::Graphic::Vulkan::Util {
    constexpr vk::QueueFlags required =
        vk::QueueFlagBits::eGraphics |
        vk::QueueFlagBits::eCompute |
        vk::QueueFlagBits::eTransfer;

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
            vva_log_info("Vulkan Graphical Device Created");
            return VulkanDevice {
                .physical_device = std::move(physical_device),
                .logical_device = std::move(logical_device),
                .device_queues = std::move(queues),
                .device_queue_coordinates = queue_selection
            };
        }

        auto SelectAllCapableQueueFamily(const vk::raii::SurfaceKHR& surface,
                                         const vk::raii::PhysicalDevice& physical_device) -> std::optional<QueueCoordinate> {
            std::vector<vk::QueueFamilyProperties2> queue_family_properties = physical_device.
                getQueueFamilyProperties2();

            for (size_t i = 0; i < queue_family_properties.size(); i++) {
                if (((queue_family_properties[i].queueFamilyProperties.queueFlags & required) == required)
                    && (physical_device.getSurfaceSupportKHR(static_cast<uint32_t>(i), *surface))) {
                    vva_log_info("Found Queue Family Capable of Graphics, Compute, Transfer, Present at index {}", i);
                    return std::optional<QueueCoordinate>({static_cast<uint32_t>(i), 0});
                }
            }
            return std::nullopt;
        }

        auto SelectDeviceQueues(const vk::raii::SurfaceKHR& surface, const vk::raii::PhysicalDevice& physical_device) -> DeviceQueueCoordinates {
            //std::vector<vk::QueueFamilyProperties> queueFamilyProperties = device.physical_device_.getQueueFamilyProperties2();
            if (const auto result = SelectAllCapableQueueFamily(surface, physical_device)) {
                const QueueCoordinate graphics = result.value();
                return {.graphics = graphics, .present = graphics, .compute = graphics, .transfer = graphics};
            }
            else {
                vva_log_error("Suitable Device not found");
                //TODO: don't directly crash the program when suitable device is not found
                exit(1);
            }
        }
    }

    auto CreateVulkanDevice(
        const vk::raii::SurfaceKHR& surface, vk::raii::PhysicalDevice physicalDevice) -> VulkanDevice {

        // find the index of the first queue family that supports graphics
        std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice.getQueueFamilyProperties();

        DeviceQueueCoordinates queue_selection = SelectDeviceQueues(surface, physicalDevice);

        float queue_priority = 0.5;
        vk::DeviceQueueCreateInfo device_queue_create_info{
            .queueFamilyIndex = queue_selection.graphics.QueueFamilyIndex,
            .queueCount = 1,
            .pQueuePriorities = &queue_priority
        };
        vk::PhysicalDeviceFeatures device_features;
        // Create a chain of feature structures
        vk::StructureChain<vk::PhysicalDeviceFeatures2,
                           vk::PhysicalDeviceVulkan11Features,
                           vk::PhysicalDeviceVulkan13Features,
                           vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>
            featureChain = {
                {}, // vk::PhysicalDeviceFeatures2 (empty for now)
                {.shaderDrawParameters = true}, // Enable shader draw parameters from Vulkan 1.1
                {.dynamicRendering = true}, // Enable dynamic rendering from Vulkan 1.3
                {.extendedDynamicState = true} // Enable extended dynamic state from the extension
            };
        std::vector<const char*> requiredDeviceExtension = {
            vk::KHRSwapchainExtensionName
        };

        vk::DeviceCreateInfo deviceCreateInfo{
            .pNext = &featureChain.get<vk::PhysicalDeviceFeatures2>(),
            .queueCreateInfoCount = 1,
            .pQueueCreateInfos = &device_queue_create_info,
            .enabledExtensionCount = static_cast<uint32_t>(requiredDeviceExtension.size()),
            .ppEnabledExtensionNames = requiredDeviceExtension.data()
        };

        return assembleVulkanDevice(std::move(physicalDevice),
            deviceCreateInfo,
            queue_selection);


    }

    uint32_t findTransferQue(uint32_t graphicsIndex,
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
