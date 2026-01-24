#pragma once

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

    GLFWwindow* m_Window;
    VkInstance  m_Instance;
};
