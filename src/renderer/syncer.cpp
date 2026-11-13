#include "syncer.hpp"

void Syncer::init(VulkanContext *vko) {
    this->vko = vko;
    createSyncObjects();
}

void Syncer::destroy() {
    for (uint32_t i = 0; i < 2; i++) {
        vkDestroySemaphore(vko->device.device, imageAvailableSemaphores[i], NULL);
        vkDestroySemaphore(vko->device.device, renderFinishedSemaphores[i], NULL);
        vkDestroyFence(vko->device.device, inFlightFences[i], NULL);
    }
}

void Syncer::createSyncObjects() {
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT; // signal start so that main loop doesnt keep waiting for it to start

    // allocate space for the sync objects
    imageAvailableSemaphores = (VkSemaphore*) malloc(sizeof(VkSemaphore) * vko->swapchain.swapchainImageCount);
    renderFinishedSemaphores = (VkSemaphore*) malloc(sizeof(VkSemaphore) * vko->swapchain.swapchainImageCount);
    inFlightFences = (VkFence*) malloc(sizeof(VkFence) * vko->swapchain.swapchainImageCount);

    for (uint32_t i = 0; i < 2; i++) {
        if (vkCreateSemaphore(vko->device.device, &semaphoreInfo, NULL, &imageAvailableSemaphores[i]) != VK_SUCCESS || 
            vkCreateSemaphore(vko->device.device, &semaphoreInfo, NULL, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(vko->device.device, &fenceInfo, NULL, &inFlightFences[i]) != VK_SUCCESS) {
            fprintf(stderr, "Failed to create sync objects.\n");
            exit(1);
        }
    }
}
