#include "engine/engine.hpp"

int main() {
    /*
    VulkanContext: device, swapchain, commandPool
    Engine: Resources(should i add this?)(images, buffers, descriptors), and simulation
    Renderer: renderPass, pipelines, framebuffers, recorder, syncer (render pass starts everything)
    so the dependency graph goes:
    ----VulkanContext
    |        |  
    |        V
    |     Engine
    |        |
    |        V
    ----->Renderer
    or smth like that
    */
    VulkanContext vko;
    vko.init();
    Engine engine;
    engine.init(&vko);
    engine.mainLoop();
    engine.destroy();
    vko.destroy();
    return 0;
}
