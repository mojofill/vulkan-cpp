#pragma once

#include "vgfx.hpp"
#include "graphics_pipeline.hpp"

namespace Compute {
    void createComputePipeline(Device *device,
        Descriptors *descriptors,
        VkPipeline *destPipeline,
        VkPipelineLayout *destPipelineLayout,
        const char *compute_path
    );
    void createImageMemoryBarrier(VkImage &image, VkCommandBuffer commandBuffer);
}
