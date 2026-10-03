#include "device.hpp"
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <memory>

void Device::init(int &width, int &height) {
    createWindow(width, height);
    createInstance();
    createPhysicalDevice();
    createSurface();
    findQueueFamilies();
    createLogcalDevice();
    createSwapchainSupport();
}

void Device::destroy() {
    glfwDestroyWindow(window);
}

void Device::createWindow(int &width, int &height) {
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW\n" << std::endl;
        exit(1);
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // necessary to use Vulkan
    // for now, set width=height=800
    width = 800;
    height = 800;
    window = glfwCreateWindow(800, 800, "Window", NULL, NULL);
    if (!window) {
        printf("Failed to create GLFW window\n");
        exit(1);
    }
}

void Device::createInstance() {
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Vulkan: Windows";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_2;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    
    uint32_t glfwExtensionCount;
    
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    uint32_t extensionCount = glfwExtensionCount;
    #ifdef __APPLE__
        extensionCount++;
    #endif
    const char **extensions = (const char **) malloc(sizeof(char*) * extensionCount);

    for (uint32_t i = 0; i < glfwExtensionCount; i++) {
        extensions[i] = glfwExtensions[i];
    }

    #ifdef __APPLE__
        extensions[glfwExtensionCount] = VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME;
    #endif

    createInfo.enabledExtensionCount = extensionCount;
    createInfo.ppEnabledExtensionNames = extensions;

    // validation layers

    const char* layers[] = {"VK_LAYER_KHRONOS_validation"};
    uint32_t layerCount = 0;
    vkEnumerateInstanceLayerProperties(&layerCount, NULL);

    VkLayerProperties* availableLayers = (VkLayerProperties*) malloc(sizeof(VkLayerProperties) * layerCount);

    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers);

    int validationLayerFound = 0;

    for (uint32_t i = 0; i < layerCount; i++) {
        if (strcmp(availableLayers[i].layerName,
                "VK_LAYER_KHRONOS_validation") == 0) {
            validationLayerFound = 1;
            break;
        }
    }

    if (validationLayerFound) {
        createInfo.enabledLayerCount = 1;
        createInfo.ppEnabledLayerNames = layers;
    } else {
        createInfo.enabledLayerCount = 0;
        createInfo.ppEnabledLayerNames = NULL;
    }

    free(availableLayers);

    #ifdef __APPLE__
        createInfo.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
    #endif

    VkResult res = vkCreateInstance(&createInfo, NULL, &instance);
    if (res != VK_SUCCESS) {
        std::cerr << "vkCreateInstance failed: " << res << std::endl;
        exit(1);
    }
}

void Device::createSurface() {
    if (glfwCreateWindowSurface(instance, window, NULL, &surface) != VK_SUCCESS) {
        fprintf(stderr, "Failed to create surface\n");
        exit(1);
    }
}

void Device::createPhysicalDevice() {
    uint32_t physicalDeviceCount = 0;

    vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, NULL);

    if (physicalDeviceCount == 0) {
        fprintf(stderr, "No Vulkan-capable GPUs found.\n");
        exit(EXIT_FAILURE);
    }

    VkPhysicalDevice *physicalDevices = (VkPhysicalDevice*) malloc(sizeof(VkPhysicalDevice) * physicalDeviceCount);

    vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices);

    int bestDevice = -1;
    int bestScore = -1;

    for (uint32_t i = 0; i < physicalDeviceCount; i++) {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(physicalDevices[i], &properties);

        int score = 0;
        // printf("  [%u] %s\n", i, properties.deviceName);

        switch (properties.deviceType) {
            case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
                score += 1000;
                break;

            case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
                score += 500;
                break;

            case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
                score += 300;
                break;

            case VK_PHYSICAL_DEVICE_TYPE_CPU:
                score += 100;
                break;

            default:
                break;
        }

        // Prefer newer Vulkan versions.
        score += properties.apiVersion;

        if (score > bestScore) {
            bestScore = score;
            bestDevice = i;
        }
    }

    if (bestDevice == -1) {
        fprintf(stderr, "Failed to select a physical \n");
        free(physicalDevices);
        exit(EXIT_FAILURE);
    }

    physicalDevice = physicalDevices[bestDevice];

    VkPhysicalDeviceProperties selectedProperties;
    vkGetPhysicalDeviceProperties(physicalDevice, &selectedProperties);

    printf("Selected GPU: %s\n", selectedProperties.deviceName);

    free(physicalDevices);
}

void Device::findQueueFamilies() {
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, NULL);
    VkQueueFamilyProperties *queueFamilyProperties = (VkQueueFamilyProperties*) malloc(sizeof(VkQueueFamilyProperties) * queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilyProperties);

    graphicsFamilyIndex = -1;
    presentFamilyIndex = -1;

    for (uint32_t i = 0; i < queueFamilyCount; i++) {
        if (queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            graphicsFamilyIndex = i;
        }

        VkBool32 presentSupport;
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);
        if (presentSupport) {
            presentFamilyIndex = i;
        }

        if (graphicsFamilyIndex != -1 && presentFamilyIndex != -1) break;
    }

    if (graphicsFamilyIndex == -1 || presentFamilyIndex == -1) {
        fprintf(stderr, "Failed to find required queue families\ngraphics family: %d\npresent family: %d", graphicsFamilyIndex, presentFamilyIndex);
        exit(1);
    }
}

void Device::createLogcalDevice() {
    uint32_t extensionCount = 1;

    #ifdef __APPLE__
        extensionCount = 2;
    #endif

    const char* deviceExtensions[2];
    deviceExtensions[0] = VK_KHR_SWAPCHAIN_EXTENSION_NAME;

    #ifdef __APPLE__
        deviceExtensions[1] = VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME;
    #endif

    // create device queue
    float queuePriority = 1.0f;

    VkDeviceQueueCreateInfo queueCreateInfos[2]{};
    queueCreateInfos[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfos[0].queueFamilyIndex = graphicsFamilyIndex;
    queueCreateInfos[0].queueCount = 1;
    queueCreateInfos[0].pQueuePriorities = &queuePriority;

    VkPhysicalDeviceFeatures deviceFeatures{};
    deviceFeatures.samplerAnisotropy = VK_TRUE; // must add device feature sampler anisotropy
    deviceFeatures.fillModeNonSolid = VK_TRUE;
    deviceFeatures.fragmentStoresAndAtomics = VK_TRUE; // turning this on only because slang doesnt allow storage images in fragment shaders without this enabled

    VkPhysicalDeviceVulkan12Features v12Features{};
    v12Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
    v12Features.separateDepthStencilLayouts = VK_TRUE;

    // create logical device
    VkDeviceCreateInfo deviceCreateInfo{};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pNext = &v12Features;
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pQueueCreateInfos = queueCreateInfos;
    deviceCreateInfo.enabledExtensionCount = extensionCount;
    deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions;
    deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

    if (vkCreateDevice(physicalDevice, &deviceCreateInfo, NULL, &device) != VK_SUCCESS) {
        fprintf(stderr, "Failed to create logical device\n");
        exit(1);
    }

    // must grab queue handle from logical device
    vkGetDeviceQueue(device, graphicsFamilyIndex, 0, &graphicsQueue);
}

void Device::createSwapchainSupport() {
    // get surface capabilities
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCapabilities);

    // pick surface format
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, NULL);
    VkSurfaceFormatKHR *formats = (VkSurfaceFormatKHR*) malloc(sizeof(VkSurfaceFormatKHR) * formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, formats);

    surfaceFormat = formats[0];

    free(formats);

    // pick present mode
    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, NULL);
    VkSurfacePresentModeKHR *presentModes = (VkSurfacePresentModeKHR*) malloc(presentModeCount * sizeof(VkSurfacePresentModeKHR));
    
    presentMode = VK_PRESENT_MODE_FIFO_KHR;
    // pick mailbox as present mode if available, otherwise fifo (always available)
    for (uint32_t i = 0; i < presentModeCount; i++) {
        if (presentModes[i].presentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
            break;
        }
    }

    free(presentModes);
}
