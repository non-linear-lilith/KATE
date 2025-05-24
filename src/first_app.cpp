#include <first_app.hpp>
#include <kate_camera.hpp>
#include <simple_render_system.hpp>
#include "input/keyboard_input.hpp"
#include <kate_buffer.hpp>

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
const float pi2 = 6.28f;

namespace kate{
    struct GlobalUbo{
        glm::mat4 projectionView{1.f};
        glm::vec3 lightDirection = glm::normalize(glm::vec3{1.f,-3.f,-1.f});
    };

    FirstApp::FirstApp(){
        globalPool = KATEDescriptorPool::Builder(app_Device)
        .setMaxSets(KATESwapChain::MAX_FRAMES_IN_FLIGHT)
        .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, KATESwapChain::MAX_FRAMES_IN_FLIGHT)
        .build();
        loadGameObjects();
        
    }
    FirstApp::~FirstApp(){
    }
    static double xpos, ypos;

    void FirstApp::run() {
        std::vector<std::unique_ptr<KATEBuffer>> uboBuffers(KATESwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < uboBuffers.size(); i++) {
            uboBuffers[i] = std::make_unique<KATEBuffer>(
                app_Device,
                sizeof(GlobalUbo),
                1,
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
                uboBuffers[i]->map();
        }
        /** *
         * Create a descriptor set layout for the global ubo 
         * with a size of 1 and a type of uniform buffer. 
         * Uniform buffers are used to pass data to the shaders. 
         * 
         * */
        auto globalSetLayout = KATEDescriptorSetLayout::Builder(app_Device)
            .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
            .build();

        /** *
         * Create a vector of descriptor sets for the global ubo
         * with a size of KATESwapChain::MAX_FRAMES_IN_FLIGHT
         * and a type of uniform buffer.
         * The descriptor sets are used to pass data to the shaders.
         * 
         * */
        std::vector<VkDescriptorSet> globalDescriptorSets(KATESwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < globalDescriptorSets.size(); i++) {
            auto bufferInfo = uboBuffers[i]->descriptorInfo();
            KATEDescriptorWriter(*globalSetLayout, *globalPool)
                .writeBuffer(0, &bufferInfo)
                .build(globalDescriptorSets[i]);
        }

        static float accumulated_frame_time = 0; //Time of frames accumulated per second to be used for the frame counter
        static uint16_t frame_counter = 0;// Number of frames that are displayed per second
        
        SimpleRenderSystem simpleRenderSystem{app_Device, appRenderer->getSwapChainRenderPass(),globalSetLayout->getDescriptorSetLayout()}; // Create the render system
        KATECamera camera{};
        //camera.setViewTarget(glm::vec3(-1.f,-1.f,1.f),glm::vec3(0.f,0.f,2.5f)); //set camera angle and position}
        auto viewerObject = KATEGameObject::createGameObject();
        KeyboardInput cameraController;
        //glfwSetInputMode(user_Window.getGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN); //HIDE CURSOR  
        
        auto currentTime{std::chrono::high_resolution_clock::now()};

        static float rad = 0.f;

        while (!user_Window.shouldClose()) {
            glfwPollEvents();    // Poll for events (keyboard, mouse, etc.)

            imguiManager->newFrame(); // Start a new ImGui frame

            // Define your ImGui UI elements
            

            auto newTime{std::chrono::high_resolution_clock::now()}; // Get the current time
            float frameTime = std::chrono::duration<float,std::chrono::seconds::period>(newTime-currentTime).count(); // Calculate the time since the last frame
            currentTime = newTime;
            frameTime = glm::min(frameTime,MAX_FRAME_TIME);
            glfwGetCursorPos(user_Window.getGLFWWindow(),&xpos,&ypos); 

            ImGui::Begin("KATE Engine Debug");
            ImGui::Text("Frame Time: %.3f ms",frameTime*1000.f);
            ImGui::Text("FPS: %.3f",1.f/frameTime);
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Mouse Position: ");
            ImGui::Text("X: %.3f",xpos);
            ImGui::Text("Y: %.3f",ypos);
            ImGui::Text("Camera Position: ");
            ImGui::Text("X: %.3f",viewerObject.transform.translation.x);
            ImGui::Text("Y: %.3f",viewerObject.transform.translation.y);
            ImGui::Text("Z: %.3f",viewerObject.transform.translation.z);
            ImGui::Text("Camera Rotation: ");
            ImGui::Text("X: %.3f",viewerObject.transform.rotation.x);
            ImGui::Text("Y: %.3f",viewerObject.transform.rotation.y);
            ImGui::Text("Rat Rotation: ");
            ImGui::Text("Angle: %.3f",rad);
            ImGui::End();
            
            cameraController.moveInPlaneXZ(user_Window.getGLFWWindow(),frameTime,viewerObject);
            camera.setViewYXZ2(viewerObject.transform.translation,viewerObject.transform.rotation);
            float aspect = appRenderer->getAspectRatio();
            //camera.setOrthographicProjection(-aspect,aspect,-1,1,-1,1); Orthographic projection
            camera.setPerspectiveProjection(glm::radians(50.f),aspect,0.1f,10.f); //set camera in perspective projections
            if (auto commandBuffer = appRenderer->beginFrame()) {
                int frameIndex = appRenderer->getFrameIndex();
                FrameInfo frameInfo{
                    frameIndex,
                    frameTime,
                    commandBuffer,
                    camera,
                    globalDescriptorSets[frameIndex]
                };
                GlobalUbo ubo = {};
                ubo.projectionView = camera.getProjection() * camera.getView();
                uboBuffers[frameIndex]->writeToBuffer(&ubo, sizeof(ubo));//test
                uboBuffers[frameIndex]->flush();

                appRenderer->beginSwapChainRenderPass(commandBuffer); // begin render pass

                simpleRenderSystem.renderGameObjects(frameInfo, gameObjects); // render game objects
                imguiManager->render(commandBuffer); // render imgui

                appRenderer->endSwapChainRenderPass(commandBuffer); // end render pass

                appRenderer->endFrame(); // end frame
            }

            gameObjects.at(0).transform.rotation = {0.f,glm::pi<float>()/2.f+rad,glm::pi<float>()};
            float frecuency = 1.0f; // Frequency of the rotation in radians per second
            rad += frecuency * frameTime; // Scale increment by frame time
            if (rad >= pi2) {
                rad -= pi2;
            }
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
            std::shared_ptr<KATEModel> cube_model = KATEModel::createModelFromFile(app_Device,"data/models/cube.obj");
            auto rat = KATEGameObject::createGameObject();
            auto cube = KATEGameObject::createGameObject();
            cube.model = cube_model; //set the model of the game object to be rendered
            cube.transform.translation = {0.f,1.f,4.f};
            cube.transform.scale = {10.f,0.2f,10.f};
            cube.color = {1.f,0.f,0.f};
            rat.model = rat_model; 
            rat.transform.translation = {0.f,.5f,4.f};
            rat.transform.rotation = {0.f,glm::pi<float>()/2.f,glm::pi<float>()};
            rat.transform.scale = glm::vec3(0.5f);
            gameObjects.push_back(std::move(rat));
            gameObjects.push_back(std::move(cube));
            gameObjects.at(0).model = rat_model; //set the model of the game object to be rendered
            gameObjects.at(1).model = cube_model; //set the model of the game object to be rendered

            //std::cout<<rat.getId();
            std::cout<<"rat loaded with "<<rat_model->getnumberOfVertices()<<"vertices \n";
            std::cout<<"cube loaded with "<<cube_model->getnumberOfVertices()<<"vertices \n";

            auto rat2 = KATEGameObject::createGameObject();
            }

}