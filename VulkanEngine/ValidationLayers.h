#pragma once

#include <vector>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

#ifdef NDEBUG
    const bool b_EnableValidationLayers = false;
#else
    const bool b_EnableValidationLayers = true;
#endif