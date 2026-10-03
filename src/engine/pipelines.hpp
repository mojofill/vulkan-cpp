#pragma once

#include "vgfx.hpp"
#include "graphics_pipeline.hpp"
#include "compute_pipeline.hpp"

class Pipelines {
private:
    Device *device;
    void setVertexBindingDescription();
	void setVertexAttributeDescriptions();
public:
    VkVertexInputBindingDescription bindingDescs[1];
	VkVertexInputAttributeDescription attrDescs[1];
    VkPipeline graphicsPipeline;
    VkPipelineLayout graphicsPipelineLayout;
    VkPipeline computePipeline;
    VkPipelineLayout computePipelineLayout;

    Pipelines() : graphicsPipeline(VK_NULL_HANDLE), graphicsPipelineLayout(VK_NULL_HANDLE) {}

    void createGraphicsPipeline(
        Device *device,
        Descriptors *descriptors,
        RenderPass *renderPass
    );
    void createComputePipeline(
        Device *device,
        Descriptors* descriptors,
        VkPipeline *destPipeline,
        VkPipelineLayout *destPipelineLayout,
        const char *compute_path
    );
    void destroy();
};