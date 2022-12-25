#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>

namespace kate //(KAT)herine (E)ngine
{
    class KATEWindow{  
        public:
            KATEWindow(int w,int h, std::string name); //constructor
            ~KATEWindow(); //destructor

            KATEWindow(const KATEWindow &) = delete;
            KATEWindow &operator=(const KATEWindow &) = delete;

            bool shouldlose(){return glfwWindowShouldClose(window);}
            VkExtent2D getExtent(){return {static_cast<uint32_t>(WIDTH),static_cast<uint32_t>(HEIGHT)};};

            void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
        private:
            void initWindow();
            const int WIDTH;
            const int HEIGHT;
            std::string windowName;
            GLFWwindow *window; 
            
    };
}