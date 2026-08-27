//
// Created by Wangsicong Wei on 2026-07-27.
//

#include "VulkanDebugMessengerUtil.h"

#include <iostream>

namespace Funccia::Graphic::Vulkan::Util {
    namespace {
        VKAPI_ATTR vk::Bool32 VKAPI_CALL debugCallback(vk::DebugUtilsMessageSeverityFlagBitsEXT severity,
                                                       vk::DebugUtilsMessageTypeFlagsEXT type,
                                                       const vk::DebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                       void*) {
            std::cerr << "validation layer: type " << to_string(type) << " msg: " << pCallbackData->pMessage <<
                std::endl;

            return vk::False;
        }
    }

    auto CreateDebugMessenger(const vk::raii::Instance& instance,
                              bool enableValidationLayers) -> std::optional<vk::raii::DebugUtilsMessengerEXT> {
        if (!enableValidationLayers) return std::nullopt;
        vk::DebugUtilsMessageSeverityFlagsEXT severityFlags(
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eError);
        vk::DebugUtilsMessageTypeFlagsEXT messageTypeFlags(
            vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
            vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);
        vk::DebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfoEXT{
            .messageSeverity = severityFlags,
            .messageType = messageTypeFlags,
            .pfnUserCallback = &debugCallback
        };
        return {instance.createDebugUtilsMessengerEXT(debugUtilsMessengerCreateInfoEXT)};
    }
}
