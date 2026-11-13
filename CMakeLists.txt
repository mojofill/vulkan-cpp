cmake_minimum_required(VERSION 3.21)
project(vk_app CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Vulkan REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

add_executable(vk_app
    src/main.cpp

    src/vk_context/device.cpp
    src/vk_context/swapchain.cpp
    src/vk_context/command_pool.cpp
    src/vk_context/vk_context.cpp

    src/engine/images.cpp
    src/engine/buffers.cpp
    src/engine/descriptors.cpp
    src/engine/resources.cpp
    src/engine/recorder.cpp
    src/engine/engine.cpp

    src/renderer/renderer.cpp
    src/renderer/renderpass.cpp
    src/renderer/graphics_pipeline.cpp
    src/renderer/compute_pipeline.cpp
    src/renderer/pipelines.cpp
    src/renderer/framebuffers.cpp
    src/renderer/syncer.cpp
)

target_include_directories(vk_app PRIVATE
    ${Vulkan_INCLUDE_DIRS}
)

target_link_libraries(vk_app PRIVATE
    Vulkan::Vulkan
    glfw
)
