#include "first_app.hpp"
#include "kate_camera.hpp"
#include "simple_render_system.hpp"
#include "input/keyboard_input.hpp"

//external libs
#include <irrklang/include/irrKlang.h>

#include <ft2build.h>
#include FT_FREETYPE_H

//glm libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/gtc/constants.hpp>
#include <glm/glm.hpp>
//std libs

#include <array>
#include <chrono>
#include <iostream>
#include <stdexcept>
#include <unistd.h>
#include <string.h>


const float MAX_FRAME_TIME = 0.1f;

namespace kate{
    
    FirstApp::FirstApp(){
        loadGameObjects();
    }
    FirstApp::~FirstApp(){
    }
    static double xpos, ypos;



    void FirstApp::run() {
        static float accumulated_frame_time = 0; //Time of frames accumulated per second to be used for the frame counter
        static uint16_t frame_counter = 0;// Number of frames that are displayed per second

        SimpleRenderSystem simpleRenderSystem{app_Device, appRenderer.getSwapChainRenderPass()};
        KATECamera camera{};
        //camera.setViewTarget(glm::vec3(-1.f,-1.f,1.f),glm::vec3(0.f,0.f,2.5f)); //set camera angle and position}
        auto viewerObject = KATEGameObject::createGameObject();
        KeyboardInput cameraController;
        //glfwSetInputMode(user_Window.getGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN); //HIDE CURSOR  
        
        auto currentTime{std::chrono::high_resolution_clock::now()};

        static float rad = 0.f;
        while (!user_Window.shouldClose()) {
            glfwPollEvents();

            auto newTime{std::chrono::high_resolution_clock::now()};
            float frameTime = std::chrono::duration<float,std::chrono::seconds::period>(newTime-currentTime).count();

            currentTime = newTime;
            frameTime = glm::min(frameTime,MAX_FRAME_TIME);
            std::cout<<1.f/frameTime<<"FPS\n";
                //glfwGetCursorPos(user_Window.getGLFWWindow(),&xpos,&ypos);  ss
                //std::cout<<"x:"<<xpos<<"| y:"<<ypos<<"\n";

            cameraController.moveInPlaneXZ(user_Window.getGLFWWindow(),frameTime,viewerObject);
            camera.setViewYXZ(viewerObject.transform.translation,viewerObject.transform.rotation);
            float aspect = appRenderer.getAspectRatio();
            //camera.setOrthographicProjection(-aspect,aspect,-1,1,-1,1); Orthograp hic projection
            camera.setPerspectiveProjection(glm::radians(50.f),aspect,0.1f,10.f); //set camera
            if (auto commandBuffer = appRenderer.beginFrame()) {
                
                appRenderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects,camera);
                appRenderer.endSwapChainRenderPass(commandBuffer);
                appRenderer.endFrame();
            }

            gameObjects.at(0).transform.rotation = {0.f,glm::pi<float>()/2.f+rad,glm::pi<float>()};
        
            rad=rad+0.06f;
            }
            vkDeviceWaitIdle(app_Device.device());
    }

    void FirstApp::loadGameObjects(){
            std::shared_ptr<KATEModel> rat_model = KATEModel::createModelFromFile(app_Device,"../data/models/rat.obj");

            auto rat = KATEGameObject::createGameObject();
            rat.model = rat_model; 
            rat.transform.translation = {0.f,.5f,4.f};
            rat.transform.rotation = {0.f,glm::pi<float>()/2.f,glm::pi<float>()};
            rat.transform.scale = glm::vec3(0.5f);
            gameObjects.push_back(std::move(rat));
            std::cout<<rat.getId();

            }

}