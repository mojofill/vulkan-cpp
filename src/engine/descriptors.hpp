#pragma once

#include "../vk_context/vk_context.hpp"
#include "buffers.hpp"
#include "images.hpp"
#include "resources.hpp"

typedef struct UniformBufferObject {
    uint32_t frameCount;
} UniformBufferObject;

class Descriptors {
private:
    VulkanContext *vko;
    Buffers *buffers;
    Images *images;
    Resources *resources;
    void addDescriptorSetLayoutBinding(
        std::vector<VkDescriptorSetLayoutBinding> &bindings,
        std::vector<VkDescriptorPoolSize> &poolSizes,
        uint32_t binding,
        VkDescriptorType descriptorType,
        VkShaderStageFlags stageFlags
    );
    void addWriteBufferDescriptorSet(
        std::vector<VkWriteDescriptorSet> &desciptorWrites,
        VkDescriptorSet descriptorSet,
        uint32_t dstBinding,
        VkDescriptorType descriptorType,
        VkDescriptorBufferInfo *pBufferInfo
    );
    void addWriteImageDescriptorSet(
        std::vector<VkWriteDescriptorSet> &descriptorWrites,
        VkDescriptorSet descriptorSet,
        uint32_t dstBinding,
        VkDescriptorType descriptorType,
        VkDescriptorImageInfo *pImageInfo
    );
    void createDescriptorSetResources();
    void createUniformBuffer();
    void createDescriptorSets();
public:
    VkDescriptorSetLayout descriptorSetLayout;
    VkDescriptorPool descriptorPool;
    VkDescriptorSet descriptorSets[2];
    void init(VulkanContext *vko, Buffers *buffers, Images *images, Resources *resources);
    void destroy();
};
