#pragma once

#include "../vk_context/vk_context.hpp"
#include "graphics_pipeline.hpp"
#include "compute_pipeline.hpp"

class Pipelines {
private:
    VulkanContext *vko;
    void setVertexBindingDescription();
	void setVertexAttributeDescriptions();
public:
    VkVertexInputBindingDescription bindingDescs[1];
	VkVertexInputAttributeDescription attrDescs[1];
    VkPipeline graphicsPipeline = VK_NULL_HANDLE;
    VkPipelineLayout graphicsPipelineLayout = VK_NULL_HANDLE;
    VkPipeline computePipeline = VK_NULL_HANDLE;
    VkPipelineLayout computePipelineLayout = VK_NULL_HANDLE;

    void init(VulkanContext *vko);

    void createGraphicsPipeline(
        Descriptors *descriptors,
        RenderPass *renderPass,
        VkPipeline *destPipeline,
        VkPipelineLayout *destPipelineLayout,
        const char *vert_path,
        const char *frag_path
    );
    void createComputePipeline(
        Descriptors* descriptors,
        VkPipeline *destPipeline,
        VkPipelineLayout *destPipelineLayout,
        const char *compute_path
    );
    void destroy();
};