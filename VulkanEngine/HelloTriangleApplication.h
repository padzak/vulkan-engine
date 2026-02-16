#pragma once

#include <stdint.h>
#include <vector>

struct GLFWwindow;
struct VkInstance_T;
typedef VkInstance_T* VkInstance;
struct VkDebugUtilsMessengerEXT_T;
typedef VkDebugUtilsMessengerEXT_T *VkDebugUtilsMessengerEXT;
struct VkPhysicalDevice_T;
typedef VkPhysicalDevice_T* VkPhysicalDevice;
struct VkDevice_T;
typedef VkDevice_T* VkDevice;
struct VkQueue_T;
typedef VkQueue_T* VkQueue;

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
    void PickPhysicalDevice();
    void CreateLogicalDevice();
    void MainLoop();
    void Cleanup();
    void SetupDebugMessenger();
    bool ValidateExtensions(const char **extensions, uint32_t requiredExtensionCount);
    bool CheckValidationLayerSupport();
    std::vector<const char*> GetRequiredExtensions();

    GLFWwindow*         m_Window;
    VkInstance          m_Instance;
    VkPhysicalDevice    m_PhysicalDevice    = nullptr;
    VkDevice            m_LogicalDevice     = nullptr;
    VkQueue             m_GraphicsQueue     = nullptr;

    // Even the debug callback in Vulkan is managed with a handle 
    // that needs to be explicitly created and destroyed
    VkDebugUtilsMessengerEXT m_DebugMessenger = nullptr;
};
