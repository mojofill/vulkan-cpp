#pragma once

#include "vgfx.hpp"
#include "device.hpp"
#include "swapchain.hpp"

class Syncer {
private:
    Device *device;
    Swapchain *swapchain;
    void createSyncObjects();
public:
    VkFence *inFlightFences;
    VkSemaphore *imageAvailableSemaphores;
    VkSemaphore *renderFinishedSemaphores;
    void init(Device *device, Swapchain *swapchain);
    void destroy();
};
