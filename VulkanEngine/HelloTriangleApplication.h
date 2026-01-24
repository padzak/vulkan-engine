#pragma once

#include <stdint.h>

struct GLFWwindow;
struct VkInstance_T;
typedef VkInstance_T* VkInstance;

class HelloTriangleApplication
{
public:
    void run()
    {
        InitWindow();
        InitVulkan();
        MainLoop();
        Cleanup();
    }

private:
    void InitWindow();
    void CreateInstance();
    void InitVulkan();
    void MainLoop();
    void Cleanup();
    int  ValidateExtensions(const char **extensions, uint32_t requiredExtensionCount);
    bool CheckValidationLayerSupport();

    GLFWwindow* m_Window;
    VkInstance  m_Instance;
};
