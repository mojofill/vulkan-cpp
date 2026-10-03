#pragma once
#include "device.hpp"
#include <vector>

class Swapchain {
private:
    Device *device;
    void createSwapchain();
    void createImageViews();
public:
    VkSwapchainKHR swapchain;
    uint32_t swapchainImageCount;
    VkImage *swapchainImages;
    VkImageView *swapchainImageViews;
    Swapchain() : device(nullptr) {}
    Swapchain(Device *device) : device(device) {}
    void init(Device *device);
    void reset();
    void destroy();

    void recreateSwapchain();
};
