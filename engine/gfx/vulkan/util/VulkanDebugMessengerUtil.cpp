//
// Created by Wangsicong Wei on 2026-07-27.
//

#include "VulkanDebugMessengerUtil.h"

#include <iostream>

namespace vva::gfx::vulkan::util {
    namespace {
        VKAPI_ATTR vk::Bool32 VKAPI_CALL debugCallback(vk::DebugUtilsMessageSeverityFlagBitsEXT severity,
                                                       vk::DebugUtilsMessageTypeFlagsEXT type,
                                                       const vk::DebugUtilsMessengerCallbackDataEXT* p_callback_data,
                                                       void*) {
            std::cerr << "validation layer: type " << to_string(type) << " msg: " << p_callback_data->pMessage <<
                std::endl;

            return vk::False;
        }
    }

    auto createDebugMessenger(const vk::raii::Instance& instance,
                              bool enable_validation_layers) -> std::optional<vk::raii::DebugUtilsMessengerEXT> {
        if (!enable_validation_layers) return std::nullopt;
        vk::DebugUtilsMessageSeverityFlagsEXT severity_flags(
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eError);
        vk::DebugUtilsMessageTypeFlagsEXT message_type_flags(
            vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
            vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);
        vk::DebugUtilsMessengerCreateInfoEXT debug_utils_messenger_create_info_ext{
            .messageSeverity = severity_flags,
            .messageType = message_type_flags,
            .pfnUserCallback = &debugCallback
        };
        return {instance.createDebugUtilsMessengerEXT(debug_utils_messenger_create_info_ext)};
    }
}
