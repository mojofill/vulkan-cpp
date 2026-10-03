#pragma once

#include "vgfx.hpp"
#include "device.hpp"
#include "images.hpp"
#include "swapchain.hpp"
#include "command_pool.hpp"
#include "descriptors.hpp"
#include "renderpass.hpp"
#include "pipelines.hpp"
#include "framebuffers.hpp"
#include "buffers.hpp"

class Recorder {
private:
    Device *device;
    Swapchain *swapchain;
    CommandPool *commandPool;
    Images *images;
    Descriptors *descriptors;
    RenderPass *renderPass;
    Pipelines *pipelines;
    Framebuffers *framebuffers;
    Buffers *buffers;

    VkViewport viewport;
    VkRect2D scissor;

    void createCommandBuffers();
public:
    VkCommandBuffer *commandBuffers;
    void init(
        Device *device,
        Swapchain *swapchain,
        CommandPool *commandPool,
        Images *images,
        Descriptors *descriptors,
        RenderPass *renderPass,
        Pipelines *pipelines,
        Framebuffers *Framebuffers,
        Buffers *buffers
    );
    void destroy();

    void recordCommands(uint32_t currentFrame);
    void resetCommands();
};
