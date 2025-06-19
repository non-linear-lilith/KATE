#include "kate_window.hpp"
#include <stdexcept> //runtime_error

#ifndef ENGINE_DIR
#define ENGINE_DIR "../"
#endif

namespace kate{
    KATEWindow::KATEWindow(int w, int h,std::string name): WIDTH{w},HEIGHT{h},windowName{name}{
        initWindow();
    }
    KATEWindow::~KATEWindow(){
        glfwDestroyWindow(window);
        glfwTerminate();
    }
    void KATEWindow::initWindow(){
        glfwInit();                                                   // Starts the glfw app
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);                 // DeSync the opengl to GLFW
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);                   // Sync GLFW to Vulkan}
        
        window = glfwCreateWindow(WIDTH, HEIGHT, windowName.c_str(), nullptr, nullptr);
        glfwSetWindowUserPointer(window,this);
        glfwSetFramebufferSizeCallback(window,framebufferResizeCallback);
    }
    void KATEWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface){
        if(glfwCreateWindowSurface(instance,window,nullptr,surface)!= VK_SUCCESS){
            throw std::runtime_error("\x1B[31mfailed to create window\033[0m");
        }
    }
    void KATEWindow::framebufferResizeCallback(GLFWwindow *window,int width, int height){
        auto aux_window = reinterpret_cast<KATEWindow*>(glfwGetWindowUserPointer(window));
        aux_window->frameBufferResized=true;
        aux_window->WIDTH = width;
        aux_window->HEIGHT = height;   
    }
}