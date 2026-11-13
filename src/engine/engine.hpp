#pragma once

#include "../vk_context/vk_context.hpp"
#include "descriptors.hpp"
#include "buffers.hpp"
#include "images.hpp"
#include "resources.hpp"
#include "../renderer/renderer.hpp"

class Engine {
private:
    VulkanContext *vko;
    Buffers buffers;
    Images images;
    Resources resources;
    Descriptors descriptors;
    Recorder recorder;
    Renderer renderer;
public:
    void init(VulkanContext *vko);
    void mainLoop();
    void destroy();
};
