#pragma once

#include "../vk_context/vk_context.hpp"
#include "buffers.hpp"

class Images {
private:
	VulkanContext *vko;
public:
	void createImage(
		VkImageType imageType,
		uint32_t width,
		uint32_t length,
		uint32_t height,
		VkFormat format,
		VkImageTiling tiling,
		VkImageUsageFlags usage,
		VkMemoryPropertyFlags properties,
		VkImage *image,
		VkDeviceMemory *imageMemory
	);
	void transitionImageLayout(
		VkImage image,
		VkFormat format,
		VkImageLayout oldLayout,
		VkImageLayout newLayout
	);
	void createImageView(
		VkImage image,
		VkFormat format,
		VkImageAspectFlags aspectFlags,
		VkImageView *imageView
	);
	void createSampler(VkSampler *pSampler);

	void init(VulkanContext *vko);
	void destroy();
};
