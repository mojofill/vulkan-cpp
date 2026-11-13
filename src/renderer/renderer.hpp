#pragma once

#include "../vk_context/vk_context.hpp"
#include "renderpass.hpp"
#include "graphics_pipeline.hpp"
#include "pipelines.hpp"
#include "framebuffers.hpp"
#include "recorder.hpp"
#include "syncer.hpp"

class Renderer {
private:
	VulkanContext *vko;
	RenderPass renderPass;
	Pipelines pipelines;
	Framebuffers framebuffers;
	Recorder recorder;
	Syncer syncer;
public:
    void init(VulkanContext *vko, Buffers *buffers, Images *images, Descriptors *descriptors, Resources *resources);
	void destroy();
	void draw(uint32_t *currentFrame);
	void run();
};
