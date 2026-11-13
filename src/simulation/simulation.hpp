#pragma once

#include "../vk_context/vk_context.hpp"
#include "../engine/descriptors.hpp"
#include "../engine/buffers.hpp"
#include "../engine/images.hpp"

class Simulation {
public:
    Buffers &buffers;
    Images &images;
    Descriptors &descriptors;
    Simulation(Buffers &buffers, Images &images, Descriptors &descriptors) : buffers(buffers), images(images), descriptors(descriptors) {}
    void update(uint32_t ping);
};
