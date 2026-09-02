//
// Created by Wangsicong Wei on 2026-07-29.
//

#pragma once


struct QueueCoordinate {
    uint32_t queue_family_index;
    uint32_t queue_index;
};

struct DeviceQueueCoordinates {
    QueueCoordinate graphics;
    QueueCoordinate present;
    QueueCoordinate compute;
    QueueCoordinate transfer;
};

struct DeviceQueues {
    vk::raii::Queue graphics_queue{nullptr};
    vk::raii::Queue present_queue{nullptr};
    vk::raii::Queue compute_queue{nullptr};
    vk::raii::Queue transfer_queue{nullptr};
};