#include "first_app.hpp"
#include "simple_render_system.hpp"
//glm libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
//std libs
#include <stdexcept>
#include <array>
#include <unistd.h>
#include <iostream>
//Cmake headers directory declaration
#ifndef ENGINE_DIR
#define ENGINE_DIR "../include"
#endif

namespace kate{
    
    FirstApp::FirstApp(){
        loadGameObjects();

    }
    FirstApp::~FirstApp(){

    }
    void FirstApp::run() {
        SimpleRenderSystem simpleRenderSystem{app_Device, appRenderer.getSwapChainRenderPass()};
        while (!user_Window.shouldClose()) {
            glfwPollEvents();

            if (auto commandBuffer = appRenderer.beginFrame()) {
                
                appRenderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects);
                appRenderer.endSwapChainRenderPass(commandBuffer);
                appRenderer.endFrame();
            }

        }
        vkDeviceWaitIdle(app_Device.device());
    }

    void FirstApp::loadGameObjects(){
            std::vector<KATEModel::Vertex> vertices{
                {{0.0f,-0.5f},{1.0f,0.0f,0.0f}},
                {{0.5f,0.5f},{0.0f,1.0f,0.0f}},
                {{-0.5f,0.5f},{0.0f,0.0f,1.0f}}
            };
            auto app_model = std::make_shared<KATEModel>(app_Device,vertices);
            auto triangle = KATEGameObject::createGameObject();
            triangle.model = app_model;
            triangle.color = {.1f, .8f, .1f};
            //triangle.transform2d.translation.x = .2f;
            //triangle.transform2d.scale = {2.f,.5f};
            triangle.transform2d.rotation = .25f * glm::two_pi<float>();
            
            vkDeviceWaitIdle(app_Device.device());
            gameObjects.push_back(std::move(triangle));
    }

}
