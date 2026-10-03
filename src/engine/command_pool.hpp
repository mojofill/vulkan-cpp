#pragma once

#include "device.hpp"
#include "swapchain.hpp"

class CommandPool {
private:
    Device *device;
    void createCommandPool();
public:
    VkCommandPool commandPool;
    CommandPool() : device(nullptr) {}
    CommandPool(Device *device) : device(device) {}
    void init(Device *device);
    void destroy();
};

VkCommandBuffer beginSingleTimeCommand(Device *device, CommandPool *commandPool);
void endSingleTimeCommand(Device *device, CommandPool *commandPool, VkCommandBuffer commandBuffer);
