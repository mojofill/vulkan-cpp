#include "swapchain.hpp"
#include <memory>

void Swapchain::init(Device *device) {
    this->device = device;
    createSwapchain();
    createImageViews();
    // swapchain = where images are stored
    // render pass = editting the iamges
    // framebuffer = where editted images get sent to be presented
}

void Swapchain::reset() {
    recreateSwapchain();
}

void Swapchain::createSwapchain() {
    // create swapchain
    VkSwapchainCreateInfoKHR swapchainInfo{};
    swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainInfo.surface = device->surface;
    swapchainInfo.presentMode = device->presentMode;
    swapchainInfo.imageExtent = device->surfaceCapabilities.currentExtent; // current window size
    swapchainInfo.imageColorSpace = device->surfaceFormat.colorSpace;
    swapchainInfo.imageFormat = device->surfaceFormat.format;
    swapchainInfo.minImageCount = device->surfaceCapabilities.minImageCount + 1; // one extra buffer - triple rendering
    if (device->surfaceCapabilities.maxImageCount > 0 && swapchainInfo.minImageCount > device->surfaceCapabilities.maxImageCount) {
        // if not device doesnt support, revert to max image count
        swapchainInfo.minImageCount = device->surfaceCapabilities.maxImageCount;
    }
    swapchainInfo.imageArrayLayers = 1;
    swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; // swapchain complies with render pass rules
    swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE; // images cannot be accessed concurrently
    swapchainInfo.preTransform = device->surfaceCapabilities.currentTransform;
    swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchainInfo.clipped = VK_TRUE; // pixels outside of scissor/window are not rendered
    swapchainInfo.oldSwapchain = VK_NULL_HANDLE;

    vkCreateSwapchainKHR(device->device, &swapchainInfo, NULL, &swapchain);

    vkGetSwapchainImagesKHR(device->device, swapchain, &swapchainImageCount, NULL);
    swapchainImages = (VkImage*) malloc(sizeof(VkImage) * swapchainImageCount);
    vkGetSwapchainImagesKHR(device->device, swapchain, &swapchainImageCount, swapchainImages);
}

void Swapchain::destroy() {
    for (uint32_t i = 0; i < swapchainImageCount; i++) {
        vkDestroyImageView(device->device, swapchainImageViews[i], NULL);
    }
    free(swapchainImages);
    vkDestroySwapchainKHR(device->device, swapchain, NULL);
}

void Swapchain::recreateSwapchain() {
    // pause until framebuffer is not minimized
    int width = 0, height = 0;
    glfwGetFramebufferSize(device->window, &width, &height);
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(device->window, &width, &height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(device->device);

    destroy();

    createSwapchain();
    createImageViews();
}

void Swapchain::createImageViews() {
    swapchainImageViews = (VkImageView*) malloc(sizeof(VkImageView) * swapchainImageCount);
    
    for (uint32_t i = 0; i < swapchainImageCount; i++) {
        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = swapchainImages[i];
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = device->surfaceFormat.format;
        viewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY; // rgba -> rgba. pure identity, no alteration
        viewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseMipLevel = 0;

        if (vkCreateImageView(device->device, &viewInfo, NULL, &swapchainImageViews[i]) != VK_SUCCESS) {
            fprintf(stderr, "Failed to create swapchain image view %d\n", i);
            exit(1);
        }
    }
}
