#include "engine.hpp"

void Engine::init() {
    device.init(width, height);
    swapchain.init(&device);
    commandPool.init(&device);
    images.init(&device, &commandPool, width, height);
    descriptors.init(&device, images);
    renderPass.init(&device);
    pipelines.createGraphicsPipeline(&device, &descriptors, &renderPass);
    pipelines.createComputePipeline(&device, &descriptors, &pipelines.computePipeline, &pipelines.computePipelineLayout, "./src/spvs/comp.spv");
    framebuffers.init(&device, &swapchain, &renderPass);
    buffers.init(&device, &commandPool);
    recorder.init(&device, &swapchain, &commandPool, &images, &descriptors, &renderPass, &pipelines, &framebuffers, &buffers);
    syncer.init(&device, &swapchain);
}

void Engine::cleanup() {
    syncer.destroy();
    recorder.destroy();
    buffers.destroy();
    framebuffers.destroy();
    pipelines.destroy();
    renderPass.destroy();
    descriptors.destroy();
    images.destroy();
    commandPool.destroy();
    swapchain.destroy();
    device.destroy();
}

void Engine::mainLoop() {
    uint32_t currentFrame = 0;
    while (!glfwWindowShouldClose(device.window)) {
        glfwPollEvents();
        if (glfwGetKey(device.window, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(device.window, VK_TRUE);
        }
        // can update uniforms and such here
        draw(&currentFrame);
    }
}

void Engine::draw(uint32_t *currentFrame) {
    // wait for previous frame to finish
    vkWaitForFences(device.device, 1, &syncer.inFlightFences[*currentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(device.device, swapchain.swapchain, UINT64_MAX, syncer.imageAvailableSemaphores[*currentFrame], VK_NULL_HANDLE, &imageIndex); // signal imageAvailableSemaphore when image is available.

    if (result == VK_ERROR_OUT_OF_DATE_KHR) { // usually happens after window resize
        swapchain.recreateSwapchain();
        framebuffers.createFramebuffers();
        return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        fprintf(stderr, "Failed to acquire swapchain image.\n");
        exit(1);
    }

    // only reset to use fence again if we are submitting work
    vkResetFences(device.device, 1, &syncer.inFlightFences[*currentFrame]);

    // if i were recording every frame, i would need to 1. vkResetCommandBuffer(commandBuffer, 0); to clear command buffer and 2. recordCommandBuffer(commandBuffer, imageIndex);
    // but since everything is pre recorded, i dont need to

    recorder.recordCommands(*currentFrame);

    // submit commands to queue
    VkSubmitInfo submitInfo{};
    // explicitly tell gpu to wait until imageAvailableSemaphore signals ready before pipeline outputs to color attachment
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT}; // first stage synced first semaphore.
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &recorder.commandBuffers[imageIndex];
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &syncer.imageAvailableSemaphores[*currentFrame]; // wait for imageAvailable semaphore to signal done before rendering
    submitInfo.pWaitDstStageMask = waitStages; // pipeline stages are synced with wait semaphores - doesnt continue onto stage until wait semaphore signals done
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &syncer.renderFinishedSemaphores[*currentFrame]; // signal renderFinished semaphore when done

    if (vkQueueSubmit(device.graphicsQueue, 1, &submitInfo, syncer.inFlightFences[*currentFrame]) != VK_SUCCESS) {
        fprintf(stderr, "Failed to submit draw command buffer to graphics queue\n");
        exit(1);
    }

    // 3. Present the rendered image
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &syncer.renderFinishedSemaphores[*currentFrame]; // wait until rendering finished
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &swapchain.swapchain;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = NULL;

    result = vkQueuePresentKHR(device.graphicsQueue, &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebuffers.framebufferResized) {
        framebuffers.framebufferResized = 0;
        swapchain.recreateSwapchain();
        framebuffers.createFramebuffers();
        // rerecord command buffers
        recorder.resetCommands();
        recorder.recordCommands(*currentFrame);
    } else if (result != VK_SUCCESS) {
        printf("Failed to present swapchain image!\n");
        exit(1);
    }

    *currentFrame = (*currentFrame + 1) % 2;
    
    vkQueueWaitIdle(device.graphicsQueue);
}

void Engine::run() {
    
}
