#include "HelloTriangleApplication.h"
#include "ValidationLayers.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <map>
#include <optional>
#include <set>
#include <stdexcept>
#include <vector>

struct QueueFamilyIndices
{
    std::optional<uint32_t> graphicsFamily;
    
    bool IsComplete() const
    {
        return graphicsFamily.has_value();
    }
};

// There are different types of queues that originate from different queue families 
// and each family of queues allows only a subset of commands. For example, there could be 
// a queue family that only allows processing of compute commands or one that only 
// allows memory transfer related commands.
static QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device)
{
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies)
    {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            indices.graphicsFamily = i;
        }

        if (indices.IsComplete()) // TODO Won't I lock myself out of using several GPUs with this early exit?
        {
            break;
        }

        i++;
    }

    return indices;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData)
{
    // Message is important enough to show
    if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) 
    {
        std::cerr << "Validation layer: " << pCallbackData->pMessage << std::endl;
    }

    return VK_FALSE;
}

static VkResult CreateDebugUtilsMessengerEXT(
    VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkDebugUtilsMessengerEXT* pDebugMessenger)
{
    // Extension functions must be loaded manually
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) 
    {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } 
    else 
    {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

static void DestroyDebugUtilsMessengerEXT(
    VkInstance instance,
    VkDebugUtilsMessengerEXT debugMessenger,
    const VkAllocationCallbacks* pAllocator)
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr)
    {
        func(instance, debugMessenger, pAllocator);
    }
}

static void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

    // Specify which severities to handle by a specific callback
    createInfo.messageSeverity =
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    // Filter which types of messages to handle
    createInfo.messageType =
        VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    // Specify the callback function
    createInfo.pfnUserCallback = DebugCallback;
    // Optional data passed to the callback
    createInfo.pUserData = nullptr;
}

static bool IsDeviceSuitable(VkPhysicalDevice device)
{
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);

    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
    QueueFamilyIndices indices = FindQueueFamilies(device);

    return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU 
        && deviceFeatures.geometryShader 
        && indices.IsComplete();
}

static int RateDeviceSuitability(VkPhysicalDevice device)
{
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    
    return deviceProperties.limits.maxImageDimension2D;
}

void HelloTriangleApplication::MainLoop()
{
    while (!glfwWindowShouldClose(m_Window)) {
        // Check for events
        glfwPollEvents();


    }
}

void HelloTriangleApplication::InitWindow()
{
    // Initialize the GLFW library
    glfwInit();

    // Do not create an OpenGL context
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    // Make the window non-resizable
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    // Initialize the window
    m_Window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
}

void HelloTriangleApplication::CreateInstance()
{
    if (b_EnableValidationLayers && !CheckValidationLayerSupport())
    { 
        throw std::runtime_error("Validation layers requested, but not available!");
    }

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
    auto glfwExtensions = GetRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(glfwExtensions.size());
    createInfo.ppEnabledExtensionNames = glfwExtensions.data();

    // Global validation layers
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (b_EnableValidationLayers) 
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();

        PopulateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
    } 
    else
    {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }

    VkResult result = vkCreateInstance(&createInfo, nullptr, &m_Instance);
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create instance!");
    }
}

void HelloTriangleApplication::InitVulkan()
{
    CreateInstance();
    SetupDebugMessenger();
    PickPhysicalDevice();
}

void HelloTriangleApplication::PickPhysicalDevice()
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(m_Instance, &deviceCount, nullptr);

    if (deviceCount == 0) 
    {
        throw std::runtime_error("Failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(m_Instance, &deviceCount, devices.data());
    

    std::multimap<int, VkPhysicalDevice> candidates;

    for (const auto& device : devices) 
    {
        if (IsDeviceSuitable(device)) 
        {
            int score = RateDeviceSuitability(device);
            candidates.insert(std::make_pair(score, device));
        }
    }

    if (candidates.rbegin()->first > 0) 
    {
        m_PhysicalDevice = candidates.rbegin()->second;
    }
    else
    {
        throw std::runtime_error("Failed to find a suitable GPU!");
    }


    //for (const auto& device : devices) 
    //{
    //    if (IsDeviceSuitable(device)) 
    //    {
    //        m_PhysicalDevice = device;
    //        break;
    //    }
    //}

    //if (m_PhysicalDevice == VK_NULL_HANDLE) 
    //{
    //    throw std::runtime_error("Failed to find a suitable GPU!");
    //}
}



void HelloTriangleApplication::Cleanup()
{
    if (b_EnableValidationLayers && m_DebugMessenger != VK_NULL_HANDLE)
    {
        // Destroy the debug messanger
        DestroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr);
    }

    // Destroy the Vulkan instance
    vkDestroyInstance(m_Instance, nullptr);

    // Destroy the window 
    glfwDestroyWindow(m_Window);

    // Terminate GLFW
    glfwTerminate();

    std::cout << "Cleanup complete\n";
}


void HelloTriangleApplication::SetupDebugMessenger()
{
    if (!b_EnableValidationLayers)
    {
        return;
    }

    VkDebugUtilsMessengerCreateInfoEXT createInfoValidation{};
    PopulateDebugMessengerCreateInfo(createInfoValidation);

    if (CreateDebugUtilsMessengerEXT(m_Instance, &createInfoValidation, nullptr, &m_DebugMessenger) != VK_SUCCESS) {
        throw std::runtime_error("failed to set up debug messenger!");
    }
}

bool HelloTriangleApplication::ValidateExtensions(const char **extensions, uint32_t requiredExtensionCount)
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

    /* -------- Alternative implementation using std::set --------
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
    -------------------------------------------------------------- */

}

bool HelloTriangleApplication::CheckValidationLayerSupport()
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    
    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : validationLayers) 
    {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers) 
        {
            if (strcmp(layerName, layerProperties.layerName) == 0) 
            {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) 
        {
            return false;
        }

    }
    
    return true;
}

std::vector<const char*> HelloTriangleApplication::GetRequiredExtensions()
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;

    // Get the required extensions from GLFW
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    
    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
    
    if (b_EnableValidationLayers) // TODO Should I check these in ValidateExtensions too?
    {
        // Setup debug extension if validation layers are enabled
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    if (!ValidateExtensions(glfwExtensions, glfwExtensionCount))
    {
        throw std::runtime_error("Extension validation failed!");
    }
    
    return extensions;
}