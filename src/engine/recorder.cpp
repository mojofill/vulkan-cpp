#include "recorder.hpp"

void Recorder::init(
    Device *device,
    Swapchain *swapchain,
    CommandPool *commandPool,
    Images *images,
    Descriptors *descriptors,
    RenderPass *renderPass,
    Pipelines *pipelines,
    Framebuffers *framebuffers,
    Buffers *buffers
) {
    this->device = device;
    this->swapchain = swapchain;
    this->commandPool = commandPool;
    this->images = images;
    this->descriptors = descriptors;
    this->renderPass = renderPass;
    this->pipelines = pipelines;
    this->framebuffers = framebuffers;
    this->buffers = buffers;
    createCommandBuffers();
}

void Recorder::createCommandBuffers() {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool->commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    commandBuffers = (VkCommandBuffer*) malloc(sizeof(VkCommandBuffer) * swapchain->swapchainImageCount);

    for (uint32_t i = 0; i < swapchain->swapchainImageCount; i++) {
        vkAllocateCommandBuffers(device->device, &allocInfo, &commandBuffers[i]);
    }
}

void Recorder::resetCommands() {
    for (uint32_t i = 0; i < swapchain->swapchainImageCount; i++) {
        vkResetCommandBuffer(commandBuffers[i], 0);
    }
}

void Recorder::recordCommands(uint32_t currentFrame) {
    // record commands (per swapchain image)
    // yo am i tripping or am i doing extra work here that i dont need to do
    for (uint32_t i = 0; i < swapchain->swapchainImageCount; i++) {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        vkBeginCommandBuffer(commandBuffers[i], &beginInfo);

        VkRenderPassBeginInfo renderPassInfoCmd{}; // most basic command is starting a render pass
        renderPassInfoCmd.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfoCmd.renderPass = renderPass->renderPass;
        renderPassInfoCmd.framebuffer = framebuffers->framebuffers[i];
        renderPassInfoCmd.renderArea.offset = (VkOffset2D){0,0};
        renderPassInfoCmd.renderArea.extent = device->surfaceCapabilities.currentExtent;

        VkClearValue clearColors[2] = {{0}, {0}};
        clearColors[0].color = (VkClearColorValue) {0.0f, 0.0f, 0.0f, 0.0f};
        clearColors[1].depthStencil = (VkClearDepthStencilValue) {1.0f, 0};
    
        renderPassInfoCmd.clearValueCount = 2;
        renderPassInfoCmd.pClearValues = clearColors;

        // first, i want use the compute shader pipeline
        vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_COMPUTE, pipelines->computePipeline);
        vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_COMPUTE, pipelines->computePipelineLayout, 0, 1, &descriptors->descriptorSets[currentFrame], 0, NULL);
        vkCmdDispatch(commandBuffers[i], 50, 50, 1); // local groups of 16x16, 800/16 = 50, thus need 50x50 groups of 16x16 local groups
        
        Compute::createImageMemoryBarrier(images->storageImage, commandBuffers[i]);

        // render pass = the "plan" that says what it will do after it gets the image data. its currently working with a color attachment as a placeholder. needs pipeline to actually supply the data to the color attachment

        vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfoCmd, VK_SUBPASS_CONTENTS_INLINE);

        // here is graphipcs pipeline work
        vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelines->graphicsPipeline);
        vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelines->graphicsPipelineLayout, 0, 1, &descriptors->descriptorSets[currentFrame], 0, NULL);

        // specified viewport + scissor to be dynamic => must explicity set them here
        viewport = (VkViewport) {0};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float) device->surfaceCapabilities.currentExtent.width;
        viewport.height = (float) device->surfaceCapabilities.currentExtent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(commandBuffers[i], 0, 1, &viewport);

        // must specify scissor as well b/c we've set pipeline to be in dynamic state
        scissor = (VkRect2D) {0};
        scissor.offset = (VkOffset2D){0,0};
        scissor.extent = device->surfaceCapabilities.currentExtent;
        vkCmdSetScissor(commandBuffers[i], 0, 1, &scissor);

        vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, &buffers->vertexBuffer, (VkDeviceSize[]) {0});
        vkCmdDraw(commandBuffers[i], 6, 1, 0, 0);

        vkCmdEndRenderPass(commandBuffers[i]);
        vkEndCommandBuffer(commandBuffers[i]);
    }
}

void Recorder::destroy() {
    // nothing to destroy
}
