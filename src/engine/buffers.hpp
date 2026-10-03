#pragma once

#include "vgfx.hpp"
#include "device.hpp"
#include "command_pool.hpp"

class Buffers {
private:
    Device *device;
public:
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;

    void init(Device *device, CommandPool *commandPool);
    void destroy();

    static uint32_t findMemoryType(Device *device, uint32_t typeFilter, VkMemoryPropertyFlags properties);
    static void createBuffer(Device *device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer *pBuffer, VkDeviceMemory *pBufferMemory);
    static void copyBuffer(Device *device, CommandPool *commandPool, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
};