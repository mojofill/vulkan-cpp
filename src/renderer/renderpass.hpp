#pragma once

#include "../vk_context/vk_context.hpp"

class RenderPass {
private:
    VulkanContext *vko;
    void createRenderPass();
public:
    VkRenderPass renderPass;
    void init(VulkanContext *vko);
    void destroy();
};
