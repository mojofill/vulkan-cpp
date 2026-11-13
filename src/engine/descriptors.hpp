#pragma once

#include "../vk_context/vk_context.hpp"
#include "buffers.hpp"
#include "images.hpp"
#include "resources.hpp"

typedef struct UniformBufferObject {
    float angle;
} UniformBufferObject;

class Descriptors {
private:
    VulkanContext *vko;
    Buffers *buffers;
    Images *images;
    Resources *resources;
    void createDescriptorSetLayout();
    void createUniformBuffer();
    void createDescriptorPool();
    void createDescriptorSets();
public:
    VkDescriptorSetLayout descriptorSetLayout;
    VkDescriptorPool descriptorPool;
    VkDescriptorSet descriptorSets[2];
    void init(VulkanContext *vko, Buffers *buffers, Images *images, Resources *resources);
    void destroy();
};
