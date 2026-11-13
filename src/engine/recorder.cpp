#include "recorder.hpp"

void Recorder::init(
    VulkanContext *vko,
    Images *images,
    Descriptors *descriptors,
    Resources *resources,
    RenderPass *renderPass,
    Pipelines *pipelines,
    Framebuffers *framebuffers,
    Buffers *buffers
) {
    this->vko = vko;
    this->images = images;
    this->descriptors = descriptors;
    this->resources = resources;
    this->renderPass = renderPass;
    this->pipelines = pipelines;
    this->framebuffers = framebuffers;
    this->buffers = buffers;
    createCommandBuffers();
}

void Recorder::createCommandBuffers() {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = vko->commandPool.commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    commandBuffers = (VkCommandBuffer*) malloc(sizeof(VkCommandBuffer) * vko->swapchain.swapchainImageCount);

    for (uint32_t i = 0; i < vko->swapchain.swapchainImageCount; i++) {
        vkAllocateCommandBuffers(vko->device.device, &allocInfo, &commandBuffers[i]);
    }
}

void Recorder::resetCommands() {
    for (uint32_t i = 0; i < vko->swapchain.swapchainImageCount; i++) {
        vkResetCommandBuffer(commandBuffers[i], 0);
    }
}

void Recorder::drawStorageImage(uint32_t currentFrame, uint32_t i) {
    VkRenderPassBeginInfo renderPassInfoCmd{}; // most basic command is starting a render pass
    renderPassInfoCmd.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfoCmd.renderPass = renderPass->renderPass;
    renderPassInfoCmd.framebuffer = framebuffers->framebuffers[i];
    renderPassInfoCmd.renderArea.offset = (VkOffset2D){0,0};
    renderPassInfoCmd.renderArea.extent = vko->device.surfaceCapabilities.currentExtent;

    VkClearValue clearColors[2] = {{0}, {0}};
    clearColors[0].color = (VkClearColorValue) {0.0f, 0.0f, 0.0f, 0.0f};
    clearColors[1].depthStencil = (VkClearDepthStencilValue) {1.0f, 0};

    renderPassInfoCmd.clearValueCount = 2;
    renderPassInfoCmd.pClearValues = clearColors;

    vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfoCmd, VK_SUBPASS_CONTENTS_INLINE);

    // here is graphipcs pipeline work
    vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelines->graphicsPipeline);
    vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelines->graphicsPipelineLayout, 0, 1, &descriptors->descriptorSets[currentFrame], 0, NULL);

    // specified viewport + scissor to be dynamic => must explicity set them here
    viewport = (VkViewport) {0};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float) vko->device.surfaceCapabilities.currentExtent.width;
    viewport.height = (float) vko->device.surfaceCapabilities.currentExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffers[i], 0, 1, &viewport);

    // must specify scissor as well b/c we've set pipeline to be in dynamic state
    scissor = (VkRect2D) {0};
    scissor.offset = (VkOffset2D){0,0};
    scissor.extent = vko->device.surfaceCapabilities.currentExtent;
    vkCmdSetScissor(commandBuffers[i], 0, 1, &scissor);

    vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, &buffers->vertexBuffer, (VkDeviceSize[]) {0});
    vkCmdDraw(commandBuffers[i], buffers->vertexCount, 1, 0, 0);
}

void Recorder::testComputeShader(uint32_t currentFrame, uint32_t i) {
    // first, i want use the compute shader pipeline
    vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_COMPUTE, pipelines->computePipeline);
    vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_COMPUTE, pipelines->computePipelineLayout, 0, 1, &descriptors->descriptorSets[currentFrame], 0, NULL);
    vkCmdDispatch(commandBuffers[i], (vko->width + 15) / 16, (vko->height + 15) / 16, 1);
    Compute::createImageMemoryBarrier(resources->storageImage, commandBuffers[i]);
}

void Recorder::recordCommands(uint32_t currentFrame) {
    // record commands (per swapchain image)
    for (uint32_t i = 0; i < vko->swapchain.swapchainImageCount; i++) {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        vkBeginCommandBuffer(commandBuffers[i], &beginInfo);

        testComputeShader(currentFrame, i);

        // draw the full screen quad
        drawStorageImage(currentFrame, i);

        vkCmdEndRenderPass(commandBuffers[i]);
        vkEndCommandBuffer(commandBuffers[i]);
    }
}

void Recorder::destroy() {
    // nothing to destroy
}
