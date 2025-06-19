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

            bool shouldClose(){
                return glfwWindowShouldClose(window);
            }
            VkExtent2D getExtent(){
                return {static_cast<uint32_t>(WIDTH),static_cast<uint32_t>(HEIGHT)};
            };
            bool wasWindowResized(){
                return frameBufferResized;
            };
            void resetWindowResizedFlag(){
                frameBufferResized=false;
            }
            void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
            GLFWwindow* getGLFWWindow() const{
                return window;
            }
        private:
            static void framebufferResizeCallback(GLFWwindow *window,int width, int height);
            void initWindow();
            int WIDTH;
            int HEIGHT;
            bool frameBufferResized = false;
            std::string windowName;
            GLFWwindow *window; 
            
    };
}