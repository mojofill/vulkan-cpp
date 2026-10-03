#include "framebuffers.hpp"

void Framebuffers::createFramebuffers() {
    framebuffers = (VkFramebuffer*) malloc(sizeof(VkFramebuffer) * swapchain->swapchainImageCount);
    
    for (uint32_t i = 0; i < swapchain->swapchainImageCount; i++) {
        // When using depth buffers, need two attachments
        // For now just one is enough
        // VkImageView attachments[2] = { vko->swapchainImageViews[i], vko->depthImageView}; // try manual array decay after
        VkImageView *attachments = &swapchain->swapchainImageViews[i];
        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass->renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = device->surfaceCapabilities.currentExtent.width;
        framebufferInfo.height = device->surfaceCapabilities.currentExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(device->device, &framebufferInfo, NULL, &framebuffers[i]) != VK_SUCCESS) {
            printf("Failed to create framebuffer %d!\n", i);
            exit(1);
        }
    }
}

void Framebuffers::framebufferResizeCallback() {
    Framebuffers *framebuffers = (Framebuffers*) glfwGetWindowUserPointer(device->window);
    framebuffers->framebufferResized = 1;
    printf("frame buffer resized!\n");
}

void Framebuffers::destroy() {
    for (int i = 0; i < 3; i++) {
        vkDestroyFramebuffer(device->device, framebuffers[i], NULL);
    }
}

void Framebuffers::init(Device *device, Swapchain *swapchain, RenderPass *renderPass) {
    this->device = device;
    this->swapchain = swapchain;
    this->renderPass = renderPass;
    glfwSetWindowUserPointer(device->window, this);
    framebuffers = (VkFramebuffer*) malloc(sizeof(VkFramebuffer) * swapchain->swapchainImageCount);
    
    for (uint32_t i = 0; i < swapchain->swapchainImageCount; i++) {
        VkImageView *attachments = &swapchain->swapchainImageViews[i];
        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass->renderPass; // still need to finish this
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = device->surfaceCapabilities.currentExtent.width;
        framebufferInfo.height = device->surfaceCapabilities.currentExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(device->device, &framebufferInfo, NULL, &framebuffers[i]) != VK_SUCCESS) {
            printf("Failed to create framebuffer %d!\n", i);
            exit(1);
        }
    }
}
