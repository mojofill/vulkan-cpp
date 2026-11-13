#include "resources.hpp"
#include <iostream>

void Resources::init(VulkanContext *vko, Buffers *buffers, Images *images) {
    this->vko = vko;
    this->buffers = buffers;
    this->images = images;
}

// width = x, length = y, height = z
void Resources::createDensityImage(int width, int length, int height) {
    images->createImage(
        VK_IMAGE_TYPE_3D,
        width,
        length,
        height,
        VK_FORMAT_R32_SFLOAT,
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        &densityImage,
        &densityMemory
    );
    images->createSampler(&densitySampler);
}

void Resources::createStorageImage(int width, int height) {
    images->createImage( // create storage image for a fullscreen quad
        VK_IMAGE_TYPE_2D,
        width,
        height,
        1,
        VK_FORMAT_R32G32B32A32_SFLOAT,
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        &storageImage,
        &storageImageMemory
    );
    images->createImageView(storageImage, VK_FORMAT_R32G32B32A32_SFLOAT, VK_IMAGE_ASPECT_COLOR_BIT, &storageImageView);
    images->createSampler(&storageSampler);
}

void Resources::destroy() {
    vkDestroyImage(vko->device.device, densityImage, NULL);
    vkDestroySampler(vko->device.device, densitySampler, NULL);
    vkFreeMemory(vko->device.device, densityMemory, NULL);
    vkDestroyImage(vko->device.device, storageImage, NULL);
    vkFreeMemory(vko->device.device, storageImageMemory, NULL);
    vkDestroyImageView(vko->device.device, storageImageView, NULL);
    vkDestroySampler(vko->device.device, storageSampler, NULL);
}
