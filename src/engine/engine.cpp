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
    renderer.init(vko, &buffers, &images, &descriptors, &resources);
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
    while (!glfwWindowShouldClose(vko->device.window)) {
        glfwPollEvents();
        if (glfwGetKey(vko->device.window, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(vko->device.window, VK_TRUE);
        }
        // i gotta port command recording from renderer (recorder - should be able to just delete the recorder) to engine (simulation)
        // can update uniforms and such here
        renderer.draw(&currentFrame);
    }
}
