#include <first_app.hpp>
#include <kate_camera.hpp>
#include <simple_render_system.hpp>
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

        SimpleRenderSystem simpleRenderSystem{app_Device, appRenderer->getSwapChainRenderPass()};
        KATECamera camera{};
        //camera.setViewTarget(glm::vec3(-1.f,-1.f,1.f),glm::vec3(0.f,0.f,2.5f)); //set camera angle and position}
        auto viewerObject = KATEGameObject::createGameObject();
        KeyboardInput cameraController;
        //glfwSetInputMode(user_Window.getGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN); //HIDE CURSOR  
        
        auto currentTime{std::chrono::high_resolution_clock::now()};

        static float rad = 0.f;




        std::cout<<"\n\ntesting imgui\n\n";
        while (!user_Window.shouldClose()) {
            glfwPollEvents();

            imguiManager->newFrame();

            // Define your ImGui UI elements
            

            auto newTime{std::chrono::high_resolution_clock::now()};
            float frameTime = std::chrono::duration<float,std::chrono::seconds::period>(newTime-currentTime).count();
            currentTime = newTime;
            frameTime = glm::min(frameTime,MAX_FRAME_TIME);
            glfwGetCursorPos(user_Window.getGLFWWindow(),&xpos,&ypos); 

            ImGui::Begin("KATE Engine Debug");
            ImGui::Text("Frame Time: %.3f ms",frameTime*1000.f);
            ImGui::Text("FPS: %.3f",1.f/frameTime);
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Mouse Position: ");
            ImGui::Text("X: %.3f",xpos);
            ImGui::Text("Y: %.3f",ypos);
            ImGui::End();
            
            cameraController.moveInPlaneXZ(user_Window.getGLFWWindow(),frameTime,viewerObject);
            camera.setViewYXZ(viewerObject.transform.translation,viewerObject.transform.rotation);
            float aspect = appRenderer->getAspectRatio();
            //camera.setOrthographicProjection(-aspect,aspect,-1,1,-1,1); Orthograp hic projection
            camera.setPerspectiveProjection(glm::radians(50.f),aspect,0.1f,10.f); //set camera
            if (auto commandBuffer = appRenderer->beginFrame()) {
                
                appRenderer->beginSwapChainRenderPass(commandBuffer);

                simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects,camera);
                imguiManager->render(commandBuffer);

                appRenderer->endSwapChainRenderPass(commandBuffer);

                appRenderer->endFrame();
            }

            gameObjects.at(0).transform.rotation = {0.f,glm::pi<float>()/2.f+rad,glm::pi<float>()};
        
            rad=rad+0.06f;
            }
            vkDeviceWaitIdle(app_Device.device());
    }
    /**
     * @brief Load every game object to be rendered using the paths to the models into the engine
     * 
     * Load the game objects to be rendered
     * 
     * @return void
     * 
     */
    void FirstApp::loadGameObjects(){
        
            std::shared_ptr<KATEModel> rat_model = KATEModel::createModelFromFile(app_Device,"data/models/rat.obj");
            auto rat = KATEGameObject::createGameObject();
            rat.model = rat_model; 
            rat.transform.translation = {0.f,.5f,4.f};
            rat.transform.rotation = {0.f,glm::pi<float>()/2.f,glm::pi<float>()};
            rat.transform.scale = glm::vec3(0.5f);
            gameObjects.push_back(std::move(rat));
            //std::cout<<rat.getId();
            std::cout<<"rat loaded\n";
            auto rat2 = KATEGameObject::createGameObject();
            }

}