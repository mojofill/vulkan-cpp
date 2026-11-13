#include "pipelines.hpp"

void Pipelines::init(VulkanContext *vko) {
    this->vko = vko;
}

void Pipelines::setVertexBindingDescription() {    
    bindingDescs[0].binding = 0; // vertex buffer binding
    bindingDescs[0].stride = sizeof(Vertex2D);
    bindingDescs[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX; // other option = instance
}

void Pipelines::setVertexAttributeDescriptions() {
    // attr 0: vec2 inPos
    attrDescs[0].binding = 0; // 0th vertex buffer
    attrDescs[0].location = 0;
    attrDescs[0].format = VK_FORMAT_R32G32_SFLOAT; // s stands for signed
    attrDescs[0].offset = offsetof(Vertex2D, pos);
}

void Pipelines::createGraphicsPipeline(
    Descriptors *descriptors,
    RenderPass *renderPass,
    VkPipeline *destPipeline,
    VkPipelineLayout *destPipelineLayout,
    const char *vert_path,
    const char *frag_path
) {
    setVertexBindingDescription();
    setVertexAttributeDescriptions();
    
    Graphics::createGraphicsPipeline(
        vko,
        descriptors,
        renderPass,
        vert_path,
        frag_path,
        VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        bindingDescs,
        sizeof(bindingDescs) / sizeof(VkVertexInputBindingDescription), // binding description count
        attrDescs,
        sizeof(attrDescs) / sizeof(VkVertexInputAttributeDescription), // attribute description count
        VK_FALSE, // depth test enable
        VK_FALSE, // depth write enable
        VK_CULL_MODE_BACK_BIT, // cull mode
        destPipeline,
        destPipelineLayout
    );
}

void Pipelines::createComputePipeline(Descriptors* descriptors, VkPipeline *destPipeline, VkPipelineLayout *destPipelineLayout, const char *compute_path) {
    Compute::createComputePipeline(vko, descriptors, destPipeline, destPipelineLayout, compute_path);
} 

void Pipelines::destroy() {
    vkDestroyPipeline(vko->device.device, graphicsPipeline, NULL);
    vkDestroyPipelineLayout(vko->device.device, graphicsPipelineLayout, NULL);
    vkDestroyPipeline(vko->device.device, computePipeline, NULL);
    vkDestroyPipelineLayout(vko->device.device, computePipelineLayout, NULL);
}
