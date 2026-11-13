#pragma once

#include "../vk_context/vk_context.hpp"

class Syncer {
private:
    VulkanContext *vko;
    void createSyncObjects();
public:
    VkFence *inFlightFences;
    VkSemaphore *imageAvailableSemaphores;
    VkSemaphore *renderFinishedSemaphores;
    void init(VulkanContext *vko);
    void destroy();
};
