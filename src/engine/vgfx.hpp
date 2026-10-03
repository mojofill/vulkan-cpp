#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_beta.h>
#define GLFW_INCLUDE_NONE // include guards for vulkan
#include <GLFW/glfw3.h>

typedef struct Vertex {
    float pos[2];
} Vertex;
