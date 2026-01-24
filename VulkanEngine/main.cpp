#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>

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
    void InitWindow() 
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

    void InitVulkan() 
    {

    }

    void MainLoop() 
    {
        while (!glfwWindowShouldClose(m_Window)) {
            // Check for events
            glfwPollEvents();


        }
    }

    void Cleanup() 
    {
        // Destroy the window 
        glfwDestroyWindow(m_Window);
        
        // Terminate GLFW
        glfwTerminate();
    }

    GLFWwindow* m_Window;
};

int main() 
{
    HelloTriangleApplication app;

    try 
    {
        app.run();
    }
    catch (const std::exception& e) 
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}