#include "HelloTriangleApplication.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>


void HelloTriangleApplication::InitWindow()
{
    // Initialize the GLFW library
    glfwInit();

    // Do not create an OpenGL context
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    // Make the window non-resizable
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    // Initialize the window
    constexpr uint32_t WIDTH = 800;
    constexpr uint32_t HEIGHT = 600;
    m_Window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
}

void HelloTriangleApplication::CreateInstance()
{
    // Provide application info - may be used by driver to optimize (optional)
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    // Create info for Vulkan instance (essential)
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    // Specify required extensions - Vulkan is a platform agnostic API 
    // and requires extensions to interface with platform specific windowing systems
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    
    // Get the required extensions from GLFW
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;

    // Global validation layers
    createInfo.enabledLayerCount = 0;

    VkResult result = vkCreateInstance(&createInfo, nullptr, &m_Instance);
    if (vkCreateInstance(&createInfo, nullptr, &m_Instance) != VK_SUCCESS) 
    {
        throw std::runtime_error("failed to create instance!");
    }
}

void HelloTriangleApplication::InitVulkan()
{
    CreateInstance();
}

void HelloTriangleApplication::MainLoop()
{
    while (!glfwWindowShouldClose(m_Window)) {
        // Check for events
        glfwPollEvents();


    }
}

void HelloTriangleApplication::Cleanup()
{
    // Destroy the window 
    glfwDestroyWindow(m_Window);

    // Terminate GLFW
    glfwTerminate();
}

GLFWwindow* m_Window;
VkInstance  m_Instance;