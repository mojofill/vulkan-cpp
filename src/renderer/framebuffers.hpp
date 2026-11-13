#pragma once

#include "../vk_context/vk_context.hpp"
#include "renderpass.hpp"

class Framebuffers {
private:
    VulkanContext *vko;
    RenderPass *renderPass;
    void framebufferResizeCallback();
public:
    uint32_t framebufferResized = 0;
    VkFramebuffer *framebuffers;
    void createFramebuffers();
    void init(VulkanContext *vko, RenderPass *renderPass);
    void destroy();
};
