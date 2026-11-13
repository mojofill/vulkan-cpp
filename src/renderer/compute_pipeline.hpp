#pragma once

#include "../vk_context/vk_context.hpp"
#include "graphics_pipeline.hpp"
#include "../engine/descriptors.hpp"

namespace Compute {
    void createComputePipeline(
        VulkanContext *vko,
        Descriptors *descriptors,
        VkPipeline *destPipeline,
        VkPipelineLayout *destPipelineLayout,
        const char *compute_path
    );
    void createImageMemoryBarrier(VkImage &image, VkCommandBuffer commandBuffer);
}
