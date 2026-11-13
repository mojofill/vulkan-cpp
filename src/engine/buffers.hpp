#pragma once

#include "../vk_context/vk_context.hpp"

class Buffers {
private:
    VulkanContext *vko;
public:
    VkBuffer uniformBuffers[2];
    VkDeviceMemory uniformBufferMemories[2];
    void *uniformBuffersMapped[2];

    uint32_t vertexCount;
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;

    void init(VulkanContext *vko);
    void destroy();

    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer *pBuffer, VkDeviceMemory *pBufferMemory);
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

    static uint32_t findMemoryType(Device *device, uint32_t typeFilter, VkMemoryPropertyFlags properties);
};