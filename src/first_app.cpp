#include "first_app.hpp"
#include "kate_camera.hpp"
#include "simple_render_system.hpp"
#include "input/keyboard_input.hpp"

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

//Cmake headers directory declaration
const float MAX_FRAME_TIME = 00.1f;

namespace kate{
    
    FirstApp::FirstApp(){
        loadGameObjects();
    }
    FirstApp::~FirstApp(){
    }
    void FirstApp::run() {
        SimpleRenderSystem simpleRenderSystem{app_Device, appRenderer.getSwapChainRenderPass()};
        KATECamera camera{};
        //camera.setViewTarget(glm::vec3(-1.f,-1.f,1.f),glm::vec3(0.f,0.f,2.5f)); //set camera angle and position}

        auto viewerObject = KATEGameObject::createGameObject();
        KeyboardInput cameraController;
        

        auto currentTime{std::chrono::high_resolution_clock::now()};
        while (!user_Window.shouldClose()) {
            glfwPollEvents();
            auto newTime{std::chrono::high_resolution_clock::now()};
            float frameTime = std::chrono::duration<float,std::chrono::seconds::period>(newTime-currentTime).count();
            currentTime = newTime;
            frameTime = glm::min(frameTime,MAX_FRAME_TIME);
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

        }
        vkDeviceWaitIdle(app_Device.device());
    } 
    std::unique_ptr<KATEModel> createCubeModel(KATEDevice& device, glm::vec3 offset) {

        std::vector<KATEModel::Vertex> vertices{
        
            // left face (white)
            {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
        
            // right face (yellow)
            {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
        
            // top face (orange, remember y axis points down)
            {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
        
            // bottom face (red)
            {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
        
            // nose face (blue)
            {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
        
            // tail face (green)
            {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
        
        };
        for (auto& v : vertices) {
            v.position += offset;
        }
        return std::make_unique<KATEModel>(device, vertices);
    }

    void FirstApp::loadGameObjects(){
            std::shared_ptr<KATEModel> cube_model = createCubeModel(app_Device,{.0f,.0f,.0f});
            auto cube = KATEGameObject::createGameObject();
            cube.model = cube_model; 
            cube.transform.translation = {.0f,.0f,2.5f};
            cube.transform.scale = {.5f,.5f,.5f};
            gameObjects.push_back(std::move(cube));

    }

}