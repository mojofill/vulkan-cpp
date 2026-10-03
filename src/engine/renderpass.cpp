#include "renderpass.hpp"

void RenderPass::init(Device *device) {
    this->device = device;
    createRenderPass();
}

void RenderPass::createRenderPass() {
    // (color/depth/any type of) attachment = describes how to use image view for a specific part of the render pass
    // image view = structured way to view or access the image (raw memory)
    VkAttachmentDescription colorAttachment = {0};
    colorAttachment.format = device->surfaceFormat.format;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR; // clear previous data
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; // image will be used for presentation

    // image layout = physical memory arrangement (what bytes go where)

    VkAttachmentReference colorAttachmentRef = {0}; // this is how a subpass gets access to the attachment
    colorAttachmentRef.attachment = 0; // pAttachments[0] - there is an array of attachments, our attachment index is 0 b/c we only have one attachment right now
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    // depth buffer attachments
    VkAttachmentDescription depthAttachment = {0};
    depthAttachment.format = VK_FORMAT_D32_SFLOAT;
    depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentRef = {0};
    depthAttachmentRef.attachment = 1; // links to around line 246 - VkAttachmentDescription attachments[2] = {colorAttachment, depthAttachment} - this is what render pass "references"
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;

    // create the subpass. right now, we just have one subpass. this is the vertex -> fragment stuff
    VkSubpassDescription subpass = {0};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS; // running GRAPHICS pipeline - not a compute pipeline
    subpass.colorAttachmentCount = 1; // one SINGULAR color output, which is attachment #0. attachment = "logical" i
    subpass.pColorAttachments = &colorAttachmentRef; // array decay, for single length array just pointer to first
    
    // Uncomment this to add depth testing for 3D simulations
    // subpass.pDepthStencilAttachment = &depthAttachmentRef;

    VkSubpassDependency dependency = {0};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL; // think negative 1 when srcSubpass is VK_SUBPASS_EXTERNAL, and max+1 when it is dstSubpass (dst = destination)
    dependency.dstSubpass = 0; // this is for the first implicit subpass from initial layout to render pass layout
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT; // only when subpass 0 finished can pipeline move onto this stage
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT; // tells last implicit subpass to wait for subpass 0 t finish before writing to color attachment

    // VkAttachmentDescription attachments[2] = {colorAttachment, depthAttachment};
    VkAttachmentDescription *attachments = &colorAttachment;

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1; // only one, cuz no depth stencil
    renderPassInfo.pAttachments = attachments;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    if (vkCreateRenderPass(device->device, &renderPassInfo, NULL, &renderPass) != VK_SUCCESS) {
        fprintf(stderr, "Failed to create render pass\n");
        exit(1);
    }
}

void RenderPass::destroy() {
    vkDestroyRenderPass(device->device, renderPass, NULL);
}
