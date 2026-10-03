#pragma once

#include "vgfx.hpp"
#include "device.hpp"
#include "swapchain.hpp"
#include "renderpass.hpp"

class Framebuffers {
private:
    Device *device;
    Swapchain *swapchain;
    RenderPass *renderPass;
    void framebufferResizeCallback();
public:
    uint32_t framebufferResized = 0;
    VkFramebuffer *framebuffers;
    void createFramebuffers();
    void init(Device *device, Swapchain *swapchain, RenderPass *renderPass);
    void destroy();
};
