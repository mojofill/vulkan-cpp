#pragma once

#include "../vk_context/vk_context.hpp"
#include "renderpass.hpp"
#include "pipelines.hpp"
#include "framebuffers.hpp"
#include "../engine/recorder.hpp"
#include "syncer.hpp"

class Renderer {
private:
	VulkanContext *vko;
	RenderPass renderPass;
	Pipelines pipelines;
	Framebuffers framebuffers;
	Syncer syncer;

	Recorder *recorder;
public:
    void init(VulkanContext *vko, Buffers *buffers, Images *images, Descriptors *descriptors, Resources *resources, Recorder *recorder);
	void destroy();
	void draw(uint32_t *currentFrame);
	void run();
};
