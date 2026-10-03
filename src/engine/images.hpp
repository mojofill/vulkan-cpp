#pragma once

#include "vgfx.hpp"
#include "device.hpp"
#include "command_pool.hpp"
#include "buffers.hpp"

class Images {
private:
	Device *device;

	static void createImage(
		Device *device,
		CommandPool *commandPool,
		uint32_t width,
		uint32_t height,
		VkFormat format,
		VkImageTiling tiling,
		VkImageUsageFlags usage,
		VkMemoryPropertyFlags properties,
		VkImage *image,
		VkDeviceMemory *imageMemory
	);
	static void transitionImageLayout(
		Device *device,
		CommandPool *commandPool,
		VkImage image,
		VkFormat format,
		VkImageLayout oldLayout,
		VkImageLayout newLayout
	);
	static void createImageView(
		Device *device,
		VkImage image,
		VkFormat format,
		VkImageAspectFlags aspectFlags,
		VkImageView *imageView
	);
	static void createSampler(Device *device, VkSampler *pSampler);
public:
	VkImage storageImage;
	VkDeviceMemory storageImageMemory;
	VkImageView storageImageView;
	VkSampler storageSampler;

	void init(Device *device, CommandPool *commandPool, int width, int height);
	void destroy();
};
