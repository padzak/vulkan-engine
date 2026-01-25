#pragma once

#include <stdint.h>
#include <vector>

struct GLFWwindow;
struct VkInstance_T;
typedef VkInstance_T* VkInstance;
//struct VkDebugUtilsMessengerEXT_T;
//typedef  VkDebugUtilsMessengerEXT_T* VkDebugUtilsMessengerEXT;
struct VkDebugUtilsMessengerEXT_T;
typedef VkDebugUtilsMessengerEXT_T *VkDebugUtilsMessengerEXT;

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
    void SetupDebugMessenger();
    bool ValidateExtensions(const char **extensions, uint32_t requiredExtensionCount);
    bool CheckValidationLayerSupport();
    std::vector<const char*> GetRequiredExtensions();

    GLFWwindow* m_Window;
    VkInstance  m_Instance;

    // Even the debug callback in Vulkan is managed with a handle 
    // that needs to be explicitly created and destroyed
    VkDebugUtilsMessengerEXT m_DebugMessenger = nullptr;
};
