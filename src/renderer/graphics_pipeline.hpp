#pragma once

#include "../vk_context/vk_context.hpp"
#include "../engine/descriptors.hpp"
#include "renderpass.hpp"
#include <assert.h>

namespace Graphics {
    void createGraphicsPipeline(
        VulkanContext *vko,
        Descriptors *descriptors,
        RenderPass *renderPass,
        const char *vert_path,
        const char *frag_path,
        VkPrimitiveTopology topology,
        VkVertexInputBindingDescription *bindingDescs,
        int bindingDescriptionCount,
        VkVertexInputAttributeDescription *attrDescs,
        int attrDesciptionCount,
        int depthTestEnable,
        int depthWriteEnable,
        VkCullModeFlagBits cullMode,
        VkPipeline *destPipeline,
        VkPipelineLayout *destPipelineLayout
    );
    VkShaderModule load_shader(VkDevice device, const char* path);
}
