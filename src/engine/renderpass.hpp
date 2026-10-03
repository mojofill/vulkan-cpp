#pragma once

#include "vgfx.hpp"
#include "device.hpp"

class RenderPass {
private:
    Device *device;
    void createRenderPass();
public:
    VkRenderPass renderPass;
    void init(Device *device);
    void destroy();
};
