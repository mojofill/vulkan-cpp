#include "engine.hpp"
#include <iostream>

void Engine::init(VulkanContext *vko) {
    this->vko = vko;
    buffers.init(vko);
    images.init(vko);
    resources.init(vko, &buffers, &images);

    // initialize all resources
    resources.createDensityImage(100, 100, 100);
    resources.createStorageImage(vko->width, vko->height);

    descriptors.init(vko, &buffers, &images, &resources);
    renderer.init(vko, &buffers, &images, &descriptors, &resources, &recorder);
}

void Engine::destroy() {
    renderer.destroy();
    descriptors.destroy();
    resources.destroy();
    images.destroy();
    buffers.destroy();
}

void Engine::mainLoop() {
    uint32_t currentFrame = 0;
    uint32_t frameCount = 0;
    while (!glfwWindowShouldClose(vko->device.window)) {
        glfwPollEvents();
        if (glfwGetKey(vko->device.window, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(vko->device.window, VK_TRUE);
        }
        // upload the uniform buffer
        UniformBufferObject uni {.frameCount = frameCount};
        memcpy(buffers.uniformBuffersMapped[currentFrame], &uni, sizeof(uni));
        frameCount++;
        renderer.draw(&currentFrame);
    }
}
