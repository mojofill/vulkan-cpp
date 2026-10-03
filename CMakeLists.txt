cmake_minimum_required(VERSION 3.21)
project(vk_app CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Vulkan REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

add_executable(vk_app
    src/main.cpp
    src/engine/engine.cpp
    src/engine/device.cpp
    src/engine/swapchain.cpp
    src/engine/command_pool.cpp
    src/engine/renderpass.cpp
    src/engine/images.cpp
    src/engine/buffers.cpp
    src/engine/descriptors.cpp
    src/engine/graphics_pipeline.cpp
    src/engine/compute_pipeline.cpp
    src/engine/pipelines.cpp
    src/engine/framebuffers.cpp
    src/engine/recorder.cpp
    src/engine/syncer.cpp
)

target_include_directories(vk_app PRIVATE
    ${Vulkan_INCLUDE_DIRS}
)

target_link_libraries(vk_app PRIVATE
    Vulkan::Vulkan
    glfw
)
