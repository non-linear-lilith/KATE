#include "first_app.hpp"

#include <stdexcept>
#include <array>
#include <unistd.h>
#include <iostream>
#ifndef ENGINE_DIR
#define ENGINE_DIR "../include"
#endif

namespace kate{
    FirstApp::FirstApp(){
        loadModels();
        createPipelineLayout();
        createPipeline();
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
    void FirstApp::loadModels(){
            std::vector<KATEModel::Vertex> vertices{{{0.0f,-0.5f}},{{0.5f,0.5f}},{{-0.5f,0.5f}}};
            appModel = std::make_unique<KATEModel>(app_Device,vertices);
    }  

    /*void FirstApp::sierpinski(std::vector<KATEModel::Vertex> &vertices,int depth,glm::vec2 left,glm::vec2 right,glm::vec2 top) {
        if (depth <= 0) {
            vertices.push_back({top});
            vertices.push_back({right});
            vertices.push_back({left});
        } else {
            auto leftTop = 0.5f * (left + top);
            auto rightTop = 0.5f * (right + top);
            auto leftRight = 0.5f * (left + right);
            sierpinski(vertices, depth - 1, left, leftRight, leftTop);
            sierpinski(vertices, depth - 1, leftRight, right, rightTop);
            sierpinski(vertices, depth - 1, leftTop, rightTop, top);
        }
    }*/
    
    void FirstApp::createPipelineLayout(){
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;
        if(vkCreatePipelineLayout(app_Device.device(),&pipelineLayoutInfo,nullptr,&pipelineLayout)!=VK_SUCCESS){
            throw std::runtime_error("\x1B[31mFATAL ERROR: Failed to create pipeline layout!\033[0m");
        }
    }
    void FirstApp::createPipeline(){
        auto pipelineConfig = KATEPipeline::defaultPipelineConfigInfo(appSwapChain.width(),appSwapChain.height());
        pipelineConfig.renderPass=appSwapChain.getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        appPipeline = std::make_unique<KATEPipeline>(app_Device,"../build/shaders/simple_shader.vert.spv","../build/shaders/simple_shader.frag.spv",pipelineConfig);
    }
    void FirstApp::createCommandBuffers(){
        commandBuffers.resize(appSwapChain.imageCount());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = app_Device.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if(vkAllocateCommandBuffers(app_Device.device(),&allocInfo,commandBuffers.data())!=VK_SUCCESS){
            throw std::runtime_error("\x1B[31mFATAL ERROR: Failed to allocate command Buffers!\033[0m");
        }
        for(int i =0;i<commandBuffers.size();i++){
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            if(vkBeginCommandBuffer(commandBuffers[i],&beginInfo) != VK_SUCCESS){
                throw std::runtime_error("\x1B[31mFATAL ERROR: Failed to begin command Buffer!\033[0m");
            }
            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = appSwapChain.getRenderPass();
            renderPassInfo.framebuffer = appSwapChain.getFrameBuffer(i);
            //RENDER AREA 
            renderPassInfo.renderArea.offset  = {0,0};
            renderPassInfo.renderArea.extent = appSwapChain.getSwapChainExtent();
            //CLEAR VALUES
            std::array<VkClearValue,2>clearValues{};
            clearValues[0].color = {0.1f,0.1f,0.1f,1.0f};
            clearValues[1].depthStencil = {1.0f, 0};

            renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();

            vkCmdBeginRenderPass(commandBuffers[i],&renderPassInfo,VK_SUBPASS_CONTENTS_INLINE);

            appPipeline->bind(commandBuffers[i]);
            appModel->bind(commandBuffers[i]);
            appModel->draw(commandBuffers[i]);

            vkCmdEndRenderPass(commandBuffers[i]);
            if(vkEndCommandBuffer(commandBuffers[i])!= VK_SUCCESS){
                throw std::runtime_error("\x1B[31mFATAL ERROR: Failed to record Command Buffer\033[0m");
            }
        }
    };
    void FirstApp::drawFrame(){
        uint32_t imageIndex;
        auto result = appSwapChain.acquireNextImage(&imageIndex);
        if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR){
            throw std::runtime_error("\x1B[31mFATAL ERROR: Failed to allocate command Buffers!\033[0m");
        }
        result = appSwapChain.submitCommandBuffers(&commandBuffers[imageIndex],&imageIndex);
        if(result!=VK_SUCCESS){
            throw std::runtime_error("\x1B[31mFATAL ERROR: Failed to present swap chain image!\033[0m");
        }
    };
}
