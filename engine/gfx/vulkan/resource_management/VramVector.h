//
// Created by Wangsicong Wei on 2026-09-09.
//

#pragma once
#include <cstdint>
#include <cstring>
#include <span>

#include "gfx/vulkan/AllocatedBuffer.h"


namespace vva::gfx::vulkan {
    ///currently each vector owns their own storage buffer,
    ///but there can be a single storage buffer that multiple VramVector<T> can use togather.
    ///TODO: implement custom arena allocation instead of one ssbo per VramVector<T>
    template <typename T>
        requires std::is_trivially_copyable_v<T>
    struct VramVector {
        static constexpr size_t element_size = sizeof(T);

        static auto create(VmaAllocator allocator, const vk::raii::Device& device, size_t number_of_elements) -> VramVector {
            auto buffer = AllocatedBuffer::createBuffer(
                allocator,
                number_of_elements * element_size,
                vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eShaderDeviceAddress,
                VMA_MEMORY_USAGE_AUTO,
                VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT
                | VMA_ALLOCATION_CREATE_MAPPED_BIT);
            vk::BufferDeviceAddressInfo address_info {.buffer = buffer.handle()};

            auto device_address = device.getBufferAddress(address_info);
            return VramVector(allocator, number_of_elements, std::move(buffer), device_address);
        }

        static auto create(VmaAllocator allocator, const vk::raii::Device& device, std::initializer_list<T> elements) -> VramVector {
            auto vec = create(allocator, device, elements.size());
            vec.insert_range(elements);
            return vec;
        }

        //TODO: find out why is this not efficient, something to do with write combine buffer?
        auto push_back(const T& element) -> void {
            //TODO: grow buffer
            //assert(current_number_of_elements_ + 1 <= current_max_elements_);
            std::memcpy(static_cast<std::byte*>(buffer_.mappedData()) + current_number_of_elements_ * element_size,
                        &element, element_size);
            current_number_of_elements_++;
        }

        auto insert_range(std::span<const T> elements) -> void {
            //assert(current_number_of_elements_ + elements.size() <= current_max_elements_);
            std::memcpy(static_cast<std::byte*>(buffer_.mappedData()) + current_number_of_elements_ * element_size,
                        elements.data(), element_size * elements.size());
            current_number_of_elements_ += elements.size();
        }

        auto clear() -> void {
            current_number_of_elements_ = 0;
        }

        [[nodiscard]] auto size() const -> size_t { return current_number_of_elements_; }
        [[nodiscard]] auto deviceAddress() const -> vk::DeviceAddress {return device_address_;}
        [[nodiscard]] auto deviceAddressAt(size_t index) -> vk::DeviceAddress {
            //assert(index <= current_number_of_elements_ );
            return device_address_ + index * element_size;
        }
        auto flush() const -> void { buffer_.flush(); }

    private:
        VramVector(VmaAllocator allocator, size_t elements, AllocatedBuffer buffer, vk::DeviceAddress device_address) :
            allocator_(allocator),
            current_max_elements_(elements),
            buffer_(std::move(buffer)),
            device_address_(device_address) {
        }

        VmaAllocator allocator_;
        size_t current_max_elements_;
        size_t current_number_of_elements_{0};
        AllocatedBuffer buffer_;
        const vk::DeviceAddress device_address_;
    };
}
