#include "compute_pipeline.hpp"

namespace Compute {
    void createComputePipeline(
        Device *device,
        Descriptors *descriptors,
        VkPipeline *destPipeline,
        VkPipelineLayout *destPipelineLayout,
        const char *compute_path
    ) {
        VkPipelineLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        layoutInfo.setLayoutCount = 1; // how many descriptor set layouts there are
        layoutInfo.pSetLayouts = &descriptors->descriptorSetLayout;
        layoutInfo.pushConstantRangeCount = 0; // not push constants for now

        if (vkCreatePipelineLayout(device->device, &layoutInfo, NULL, destPipelineLayout) != VK_SUCCESS) {
            printf("failed to create compute pipeline layout\n");
            exit(1);
        }

        VkShaderModule module = Graphics::load_shader(device->device, compute_path);

        VkPipelineShaderStageCreateInfo shaderStageInfo{};
        shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStageInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
        shaderStageInfo.module = module;
        shaderStageInfo.pName = "main";
        
        VkComputePipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
        pipelineInfo.layout = *destPipelineLayout;
        pipelineInfo.stage  = shaderStageInfo;

        if (vkCreateComputePipelines(device->device, VK_NULL_HANDLE, 1, &pipelineInfo, NULL, destPipeline) != VK_SUCCESS) {
            printf("failed to create compute pipeline\n");
            exit(1);
        }

        vkDestroyShaderModule(device->device, module, NULL);
    }

    void createImageMemoryBarrier(VkImage &image, VkCommandBuffer commandBuffer) {
        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = VK_IMAGE_LAYOUT_GENERAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_GENERAL; // more expensive to keep switching back & forth from general to optimal
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = image;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;
        // synchronization settings
        barrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        vkCmdPipelineBarrier(
            commandBuffer,
            VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
            VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
            0, 0, NULL, 0, NULL, 1, &barrier
        );
    }
}
