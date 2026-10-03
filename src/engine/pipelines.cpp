#include "pipelines.hpp"

void Pipelines::setVertexBindingDescription() {    
    bindingDescs[0].binding = 0; // vertex buffer binding
    bindingDescs[0].stride = sizeof(Vertex);
    bindingDescs[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX; // other option = instance
}

void Pipelines::setVertexAttributeDescriptions() {
    // attr 0: vec2 inPos
    attrDescs[0].binding = 0; // 0th vertex buffer
    attrDescs[0].location = 0;
    attrDescs[0].format = VK_FORMAT_R32G32_SFLOAT; // s stands for signed
    attrDescs[0].offset = offsetof(Vertex, pos);
}

void Pipelines::createGraphicsPipeline(
    Device *device,
    Descriptors *descriptors,
    RenderPass *renderPass
) {
    setVertexBindingDescription();
    setVertexAttributeDescriptions();
    this->device = device;
    Graphics::createGraphicsPipeline(
        device,
        descriptors,
        renderPass,
        "./src/spvs/vert.spv",
        "./src/spvs/frag.spv",
        VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        bindingDescs,
        sizeof(bindingDescs) / sizeof(VkVertexInputBindingDescription), // binding description count
        attrDescs,
        sizeof(attrDescs) / sizeof(VkVertexInputAttributeDescription), // attribute description count
        VK_FALSE, // depth test enable
        VK_FALSE, // depth write enable
        VK_CULL_MODE_BACK_BIT, // cull mode
        &graphicsPipeline,
        &graphicsPipelineLayout
    );
}

void Pipelines::createComputePipeline(Device *device, Descriptors* descriptors, VkPipeline *destPipeline, VkPipelineLayout *destPipelineLayout, const char *compute_path) {
    Compute::createComputePipeline(device, descriptors, destPipeline, destPipelineLayout, compute_path);
} 

void Pipelines::destroy() {
    vkDestroyPipeline(device->device, graphicsPipeline, NULL);
    vkDestroyPipelineLayout(device->device, graphicsPipelineLayout, NULL);
    vkDestroyPipeline(device->device, computePipeline, NULL);
    vkDestroyPipelineLayout(device->device, computePipelineLayout, NULL);
}
