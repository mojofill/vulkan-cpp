#pragma once

#include "../vk_context/vk_context.hpp"
#include "buffers.hpp"
#include "images.hpp"
#include "descriptors.hpp"
#include "resources.hpp"
#include "../renderer/renderpass.hpp"
#include "../renderer/pipelines.hpp"
#include "../renderer/framebuffers.hpp"

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

    // compute shader functions
    void testComputeShader(uint32_t currentFrame, uint32_t i);

    void drawStorageImage(uint32_t currentFrame, uint32_t i);
    void recordCommands(uint32_t currentFrame);
    void resetCommands();
};
