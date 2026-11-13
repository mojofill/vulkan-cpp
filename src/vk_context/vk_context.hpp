#pragma once

#include "device.hpp"
#include "swapchain.hpp"
#include "command_pool.hpp"

class VulkanContext {
public:
    int width;
    int height;
    Device device;
    Swapchain swapchain;
    CommandPool commandPool;
    void init();
    void destroy();
    VkCommandBuffer beginSingleTimeCommand();
    void endSingleTimeCommand(VkCommandBuffer commandBuffer);
};