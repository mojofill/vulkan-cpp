#include "vk_context.hpp"

void VulkanContext::init() {
    device.init(width, height);
    swapchain.init(&device);
    commandPool.init(&device);
}

void VulkanContext::destroy() {
    commandPool.destroy();
    swapchain.destroy();
    device.destroy();
}

VkCommandBuffer VulkanContext::beginSingleTimeCommand() {
    return commandPool.beginSingleTimeCommand(&device, &commandPool);
}

void VulkanContext::endSingleTimeCommand(VkCommandBuffer commandBuffer) {
    commandPool.endSingleTimeCommand(&device, &commandPool, commandBuffer);
}
