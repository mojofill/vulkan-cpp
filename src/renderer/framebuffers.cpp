#include "framebuffers.hpp"

void Framebuffers::createFramebuffers() {
    framebuffers = (VkFramebuffer*) malloc(sizeof(VkFramebuffer) * vko->swapchain.swapchainImageCount);
    
    for (uint32_t i = 0; i < vko->swapchain.swapchainImageCount; i++) {
        // When using depth buffers, need two attachments
        // For now just one is enough
        // VkImageView attachments[2] = { vko->swapchainImageViews[i], vko->depthImageView}; // try manual array decay after
        VkImageView *attachments = &vko->swapchain.swapchainImageViews[i];
        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass->renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = vko->device.surfaceCapabilities.currentExtent.width;
        framebufferInfo.height = vko->device.surfaceCapabilities.currentExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(vko->device.device, &framebufferInfo, NULL, &framebuffers[i]) != VK_SUCCESS) {
            printf("Failed to create framebuffer %d!\n", i);
            exit(1);
        }
    }
}

void Framebuffers::framebufferResizeCallback() {
    Framebuffers *framebuffers = (Framebuffers*) glfwGetWindowUserPointer(vko->device.window);
    framebuffers->framebufferResized = 1;
    printf("frame buffer resized!\n");
}

void Framebuffers::destroy() {
    for (int i = 0; i < 3; i++) {
        vkDestroyFramebuffer(vko->device.device, framebuffers[i], NULL);
    }
}

void Framebuffers::init(VulkanContext *vko, RenderPass *renderPass) {
    this->vko = vko;
    this->renderPass = renderPass;
    glfwSetWindowUserPointer(vko->device.window, this);
    framebuffers = (VkFramebuffer*) malloc(sizeof(VkFramebuffer) * vko->swapchain.swapchainImageCount);
    
    for (uint32_t i = 0; i < vko->swapchain.swapchainImageCount; i++) {
        VkImageView *attachments = &vko->swapchain.swapchainImageViews[i];
        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass->renderPass; // still need to finish this
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = vko->device.surfaceCapabilities.currentExtent.width;
        framebufferInfo.height = vko->device.surfaceCapabilities.currentExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(vko->device.device, &framebufferInfo, NULL, &framebuffers[i]) != VK_SUCCESS) {
            printf("Failed to create framebuffer %d!\n", i);
            exit(1);
        }
    }
}
