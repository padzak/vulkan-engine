#include "HelloTriangleApplication.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <set>
#include <vector>


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


    ValidateExtensions(glfwExtensions, glfwExtensionCount);
    

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
    // Destroy the Vulkan instance
    vkDestroyInstance(m_Instance, nullptr);

    // Destroy the window 
    glfwDestroyWindow(m_Window);

    // Terminate GLFW
    glfwTerminate();
}

int HelloTriangleApplication::ValidateExtensions(const char **extensions, uint32_t requiredExtensionCount)
{
    // Validate available extensions (optional)
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    // Array holding extension details
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    // Query extension details
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensions.data());

    std::cout << "Available extensions: " << availableExtensions.size() << '\n';
    for (const auto& extension : availableExtensions)
    {
        std::cout << '\t' << extension.extensionName << '\n';
    }

    std::cout << "\nRequired extensions validation:\n";
    for (uint32_t i = 0; i < requiredExtensionCount; i++)
    {
        auto it = std::find_if(availableExtensions.begin(), availableExtensions.end(),
            [&](const VkExtensionProperties& ext) {
                return std::strcmp(ext.extensionName, extensions[i]) == 0;
            });

        bool found = (it != availableExtensions.end());
        std::cout << '\t' << extensions[i] << (found ? " [OK]" : " [MISSING]") << '\n';

        if (!found)
        {
            throw std::runtime_error("Missing required Vulkan extension!");
        }
    }

    /*
    // Create a set of available extension names for O(log n) lookup
    std::set<std::string> availableSet;
    for (const auto& extension : availableExtensions)
    {
        availableSet.insert(extension.extensionName);
    }

    std::cout << "Available extensions: " << availableSet.size() << '\n';
    for (const auto& name : availableSet) 
    {
        std::cout << '\t' << name << '\n';
    }

    // Verify all required extensions are available
    std::cout << "\nRequired extensions validation:\n";
    for (uint32_t i = 0; i < requiredExtensionCount; i++) 
    {
        bool found = availableSet.find(extensions[i]) != availableSet.end();
        std::cout << '\t' << extensions[i] << (found ? " [OK]" : " [MISSING]") << '\n';

        if (!found) 
        {
            throw std::runtime_error("Missing required Vulkan extension!");
        }
    }
    */

}

GLFWwindow* m_Window;
VkInstance  m_Instance;