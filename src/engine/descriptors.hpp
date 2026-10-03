#pragma once

#include "vgfx.hpp"
#include "device.hpp"
#include "buffers.hpp"
#include "images.hpp"

typedef struct UniformBufferObject {
    float angle;
} UniformBufferObject;

class Descriptors {
private:
    // maybe dont need this if uniform buffers are only constants
    VkBuffer uniformBuffers[2];
    VkDeviceMemory uniformBufferMemories[2];
    Device *device;
    void *uniformBuffersMapped[2];
    void createDescriptorSetLayout();
    void createUniformBuffer();
    void createDescriptorPool();
    void createDescriptorSets(Images &images);
public:
    VkDescriptorSetLayout descriptorSetLayout;
    VkDescriptorPool descriptorPool;
    VkDescriptorSet descriptorSets[2];
    void init(Device *device, Images &images);
    void destroy();
};
