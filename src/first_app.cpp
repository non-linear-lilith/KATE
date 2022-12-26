#include "first_app.hpp"
#include <stdexcept>
#ifndef ENGINE_DIR
#define ENGINE_DIR "../include"
#endif

namespace kate{
    FirstApp::FirstApp(){
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
        }
    }  
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
    void FirstApp::createCommandBuffers(){};
    void FirstApp::drawFrame(){};
}
