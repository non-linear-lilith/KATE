#include "first_app.hpp"
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
    struct SimplePushConstantData{
        glm::mat2 transform{1.f};
        glm::vec2 offset;
        alignas(16) glm::vec3 color; 
    };
    FirstApp::FirstApp(){
        loadGameObjects();
        createPipelineLayout();
        recreateSwapChain();
        createCommandBuffers();
    }
    FirstApp::~FirstApp(){
        vkDestroyPipelineLayout(app_Device.device(),pipelineLayout,nullptr);
    }
    void FirstApp::run(){
        while(!user_Window.shouldlose()){
            glfwPollEvents();
            drawFrame();
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
            
            gameObjects.push_back(std::move(triangle));
    }
    void FirstApp::createPipelineLayout(){
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(SimplePushConstantData);

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
        if(vkCreatePipelineLayout(app_Device.device(),&pipelineLayoutInfo,nullptr,&pipelineLayout)!=VK_SUCCESS){
            throw std::runtime_error("\x1B[31mFATAL ERROR: Failed to create pipeline layout!\033[0m");
        }
    }
    void FirstApp::createPipeline(){
        assert(appSwapChain!=nullptr && "Cannot create a pipeline before creating the swapchain framework");
        assert(pipelineLayout!=nullptr && "Cannot create a pipeline before theres a existing layout");
        PipelineConfigInfo pipelineConfig{};
        KATEPipeline::defaultPipelineConfigInfo(pipelineConfig);
        pipelineConfig.renderPass=appSwapChain->getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        appPipeline = std::make_unique<KATEPipeline>(app_Device,"../build/shaders/simple_shader.vert.spv","../build/shaders/simple_shader.frag.spv",pipelineConfig);
    }
    void FirstApp::freeCommandBuffers(){
        vkFreeCommandBuffers(app_Device.device(),app_Device.getCommandPool(),static_cast<float>(commandBuffers.size()),commandBuffers.data());
        commandBuffers.clear();
    }
    void FirstApp::recreateSwapChain(){
        auto extent = user_Window.getExtent();
        while(extent.width==0||extent.height==0){
            extent = user_Window.getExtent();
            glfwWaitEvents();
        }
        vkDeviceWaitIdle(app_Device.device());
        if(appSwapChain=nullptr){
            appSwapChain = std::make_unique<KATESwapChain>(app_Device,extent);
        } else{
            appSwapChain = std::make_unique<KATESwapChain>(app_Device,extent,std::move(appSwapChain));
            if(appSwapChain->imageCount()!=commandBuffers.size()){
                freeCommandBuffers();
                createCommandBuffers();
            }
        }
        appSwapChain = std::make_unique<KATESwapChain>(app_Device,extent);
        createPipeline();
    }
    void FirstApp::createCommandBuffers(){
        commandBuffers.resize(appSwapChain->imageCount());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = app_Device.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());//commandBufferCount ok

        if(vkAllocateCommandBuffers(app_Device.device(),&allocInfo,commandBuffers.data())!=VK_SUCCESS){
            throw std::runtime_error("\x1B[31mFATAL ERROR: Failed to allocate command Buffers!\033[0m");
        }
    };
    void FirstApp::recordCommandBuffer(int imageIndex){
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO; 

        if(vkBeginCommandBuffer(commandBuffers[imageIndex],&beginInfo) != VK_SUCCESS){
            throw std::runtime_error("\x1B[31mFATAL ERROR: Failed to begin command Buffer!\033[0m");
        }
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = appSwapChain->getRenderPass();
        renderPassInfo.framebuffer = appSwapChain->getFrameBuffer(imageIndex);
        //RENDER AREA
        renderPassInfo.renderArea.offset  = {0,0};
        renderPassInfo.renderArea.extent = appSwapChain->getSwapChainExtent();
        //CLEAR VALUES
        std::array<VkClearValue,2>clearValues{};
        clearValues[0].color = {0.01f,0.01f,0.01f,1.0f};
        clearValues[1].depthStencil = {1.0f, 0};

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffers[imageIndex],&renderPassInfo,VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(appSwapChain->getSwapChainExtent().width);
        viewport.height = static_cast<float>(appSwapChain->getSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor{{0, 0}, appSwapChain->getSwapChainExtent()};
        vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
        vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

        renderGameObjects(commandBuffers[imageIndex]);

        vkCmdEndRenderPass(commandBuffers[imageIndex]);
        if(vkEndCommandBuffer(commandBuffers[imageIndex])!= VK_SUCCESS){
            throw std::runtime_error("\x1B[31mFATAL ERROR: Failed to record Command Buffer\033[0m");
        }
    }

    void FirstApp::renderGameObjects(VkCommandBuffer commandBuffer){
        appPipeline->bind(commandBuffer);
        for(auto& obj:gameObjects){
            obj.transform2d.rotation = glm::mod(obj.transform2d.rotation + 0.01f,glm::two_pi<float>());

            SimplePushConstantData push{};
            push.offset = obj.transform2d.translation;
            push.color = obj.color;
            push.transform = obj.transform2d.mat2();
            vkCmdPushConstants(commandBuffer,pipelineLayout,VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,0,sizeof(SimplePushConstantData),&push);
            obj.model->bind(commandBuffer);
            obj.model->draw(commandBuffer);
        }
    }

    void FirstApp::drawFrame(){
        uint32_t imageIndex;
        auto result = appSwapChain->acquireNextImage(&imageIndex);
        if(result==VK_ERROR_OUT_OF_DATE_KHR){
            return;
        }
        if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR){
            throw std::runtime_error("\x1B[31mFATAL ERROR: Failed to allocate command Buffers!\033[0m");
        }
        recordCommandBuffer(imageIndex);
        result = appSwapChain->submitCommandBuffers(&commandBuffers[imageIndex],&imageIndex);
        if(result==VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || user_Window.wasWindowResized()){
            user_Window.resetWindowResizedFlag();
            recreateSwapChain();
            return;
        }
        if(result!=VK_SUCCESS){
            throw std::runtime_error("\x1B[31mFATAL ERROR: Failed to present swap chain image!\033[0m");
        }
    };
}
