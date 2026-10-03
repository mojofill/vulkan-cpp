#ifndef VK_ENGINE_H
#define VK_ENGINE_H

#include "vgfx.hpp"
#include "device.hpp"
#include "buffers.hpp"
#include "swapchain.hpp"
#include "command_pool.hpp"
#include "descriptors.hpp"
#include "renderpass.hpp"
#include "images.hpp"
#include "graphics_pipeline.hpp"
#include "pipelines.hpp"
#include "framebuffers.hpp"
#include "recorder.hpp"
#include "syncer.hpp"

class Engine {
private:
	int width;
	int height;
	Device device;
	Swapchain swapchain;
	CommandPool commandPool;
	Images images;
	Descriptors descriptors;
	RenderPass renderPass;
	Pipelines pipelines;
	Framebuffers framebuffers;
	Buffers buffers;
	Recorder recorder;
	Syncer syncer;
public:
    void init();
	void cleanup();
	void mainLoop();
	void draw(uint32_t *currentFrame);
	void run();
};

#endif