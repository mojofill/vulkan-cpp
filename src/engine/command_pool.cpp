#include "command_pool.hpp"
#include "vgfx.hpp"

void CommandPool::createCommandPool() {
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = device->graphicsFamilyIndex;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    if (vkCreateCommandPool(device->device, &poolInfo, NULL, &commandPool) != VK_SUCCESS) {
        printf("Failed to create command pool\n");
        exit(1);
    }
}

void CommandPool::init(Device *device) {
    this->device = device;
    createCommandPool();
}

void CommandPool::destroy() {
    vkDestroyCommandPool(device->device, commandPool, NULL);
}

VkCommandBuffer beginSingleTimeCommand(Device *device, CommandPool *commandPool) {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = commandPool->commandPool; // may want to create separate command pool for short lived commands
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(device->device, &allocInfo, &commandBuffer);

    // begin recording copy command
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT; // only using buffer once, may help give vulkan optimizations

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

void endSingleTimeCommand(Device *device, CommandPool *commandPool, VkCommandBuffer commandBuffer) {
    vkEndCommandBuffer(commandBuffer); // end recording

    // must submit command
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(device->graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(device->graphicsQueue); // BAD! do not do this.
    // printf("(queue wait single time cmd) elapsed ms: %f\n", elapsed_ms);

    // one time use, thus immediately free buffer

    vkFreeCommandBuffers(device->device, commandPool->commandPool, 1, &commandBuffer);
}
