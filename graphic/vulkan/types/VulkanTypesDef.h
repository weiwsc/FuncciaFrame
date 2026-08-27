//
// Created by Wangsicong Wei on 2026-07-29.
//

#pragma once


struct QueueCoordinate {
    uint32_t QueueFamilyIndex;
    uint32_t QueueIndex;
};

struct DeviceQueueCoordinates {
    QueueCoordinate graphics;
    QueueCoordinate present;
    QueueCoordinate compute;
    QueueCoordinate transfer;
};

struct DeviceQueues {
    vk::raii::Queue graphics_queue_{nullptr};
    vk::raii::Queue present_queue_{nullptr};
    vk::raii::Queue compute_queue_{nullptr};
    vk::raii::Queue transfer_queue_{nullptr};
};