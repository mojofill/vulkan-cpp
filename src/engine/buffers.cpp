#include "buffers.hpp"

void Buffers::destroy() {
    vkDestroyBuffer(vko->device.device, vertexBuffer, NULL);
    vkFreeMemory(vko->device.device, vertexBufferMemory, NULL);
}

void Buffers::init(VulkanContext *vko) {
    this->vko = vko;

    // staging
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingMemory;

    // recall that order of vertices is important as well. need to make sure right hand rule works
    // convention is counter clockwise = front
    // y is flipped, so -1 is top and 1 is bottom
    Vertex2D vertices[] = {   // don't need color
        { { -1.0f, -1.0f } }, // { 1.0f, 0.0f, 0.0f } }, // top left
        { { -1.0f,  1.0f } }, // { 0.0f, 0.0f, 1.0f } }, // bottom left
        { {  1.0f,  1.0f } }, // { 0.0f, 1.0f, 0.0f } }, // bottom right
        { {  1.0f,  1.0f } }, // { 1.0f, 0.0f, 0.0f } }, // bottom right
        { {  1.0f, -1.0f } }, // { 0.0f, 0.0f, 1.0f } }  // top right
        { { -1.0f, -1.0f } }, // { 0.0f, 1.0f, 0.0f } }, // top left
    };

    vertexCount = sizeof(vertices) / sizeof(Vertex2D);

    // staging buffer must be host visible and host coherent so it can be copied
    createBuffer(sizeof(vertices), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &stagingBuffer, &stagingMemory);

    // copy data onto staging buffer
    void *data;
    vkMapMemory(vko->device.device, stagingMemory, 0, sizeof(vertices), 0, &data);
    memcpy(data, vertices, sizeof(vertices));
    vkUnmapMemory(vko->device.device, stagingMemory);

    // actual vertex buffer
    createBuffer(sizeof(vertices), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &vertexBuffer, &vertexBufferMemory);

    // copy from staging to test buffer
    copyBuffer(stagingBuffer, vertexBuffer, sizeof(vertices));

    // destroy staging buffer
    vkDestroyBuffer(vko->device.device, stagingBuffer, NULL);
    vkFreeMemory(vko->device.device, stagingMemory, NULL);
}

uint32_t Buffers::findMemoryType(Device *device, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(device->physicalDevice, &memProperties);
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    fprintf(stderr, "Failed to find suitable memory\n");
    exit(1);
}

void Buffers::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer *pBuffer, VkDeviceMemory *pBufferMemory) {
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size; // for now, 3 vertices
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (pBuffer == NULL) {
        printf("bad buffer pointer\n");
        exit(1);
    }

    if (vkCreateBuffer(vko->device.device, &bufferInfo, NULL, pBuffer) != VK_SUCCESS) {
        fprintf(stderr, "Failed to create vertex buffer\n");
        exit(1);
    }

    // allocate memory for buffer
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(vko->device.device, *pBuffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(&vko->device, memRequirements.memoryTypeBits, properties);
    
    if (vkAllocateMemory(vko->device.device, &allocInfo, NULL, pBufferMemory) != VK_SUCCESS) {
        fprintf(stderr, "Failed to allocate memory for vertex buffer\n");
        exit(1);
    }

    vkBindBufferMemory(vko->device.device, *pBuffer, *pBufferMemory, 0);
}

// should this be in buffers, command_pool, or recorder?
void Buffers::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
    // must create, record, and submit a copy buffer command
    VkCommandBuffer commandBuffer = vko->beginSingleTimeCommand();

    VkBufferCopy copyRegion = {0};
    copyRegion.size = size;

    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
    
    vko->endSingleTimeCommand(commandBuffer);
}