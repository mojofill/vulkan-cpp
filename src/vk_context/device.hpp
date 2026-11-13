#pragma once
#include "vgfx.hpp"

class Device {
private:
    VkInstance instance;
    uint32_t presentFamilyIndex = -1;
    uint32_t queueFamilyCount = -1;
    void createWindow(int &width, int &height);
    void createInstance();
    void createSurface();
    void createPhysicalDevice();
    void findQueueFamilies();
    void createLogcalDevice();
    void createSwapchainSupport();
public:
    VkPhysicalDevice physicalDevice;
    VkDevice device;
    VkSurfaceKHR surface;
    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    VkSurfaceFormatKHR surfaceFormat;
    VkPresentModeKHR presentMode;
    uint32_t graphicsFamilyIndex = -1;
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    GLFWwindow* window;
    void destroy();
    void init(int &width, int &height);
};