#include "first_app.hpp"

#ifndef ENGINE_DIR
#define ENGINE_DIR "../include"
#endif

namespace kate{
    void FirstApp::run(){
        while(!user_Window.shouldlose()){
            glfwPollEvents();
        }
    }
}
