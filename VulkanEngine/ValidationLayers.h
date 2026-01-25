#pragma once

#include <vector>

constexpr uint32_t WIDTH = 800; // TODO These should be moved elsewhere
constexpr uint32_t HEIGHT = 600;

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

#ifdef NDEBUG
    constexpr bool b_EnableValidationLayers = false;
#else
    constexpr bool b_EnableValidationLayers = true;
#endif