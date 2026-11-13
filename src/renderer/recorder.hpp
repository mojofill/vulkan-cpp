#pragma once

#include "../vk_context/vk_context.hpp"
#include "../engine/buffers.hpp"
#include "../engine/images.hpp"
#include "../engine/descriptors.hpp"
#include "../engine/resources.hpp"
#include "renderpass.hpp"
#include "pipelines.hpp"
#include "framebuffers.hpp"

class Recorder {
private:
    VulkanContext *vko;
    Images *images;
    Descriptors *descriptors;
    Resources *resources;
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
        VulkanContext *vko,
        Images *images,
        Descriptors *descriptors,
        Resources *resources,
        RenderPass *renderPass,
        Pipelines *pipelines,
        Framebuffers *Framebuffers,
        Buffers *buffers
    );
    void destroy();

    void recordCommands(uint32_t currentFrame);
    void resetCommands();
};
