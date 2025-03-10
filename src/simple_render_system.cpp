#include "simple_render_system.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <array>
#include <cassert>
#include <stdexcept>

#include <iostream>

#ifndef ENGINE_DIR
#define ENGINE_DIR "../include"
#endif
namespace kate {

  struct SimplePushConstantData {
    glm::mat4 transform{1.f};
    glm::mat4 normalMatrix{1.f};
  };

  SimpleRenderSystem::SimpleRenderSystem(KATEDevice& device, VkRenderPass renderPass) : app_Device{device} {
    createPipelineLayout();
    createPipeline(renderPass);
  }

  SimpleRenderSystem::~SimpleRenderSystem() {
    vkDestroyPipelineLayout(app_Device.device(), pipelineLayout, nullptr);
  }

  void SimpleRenderSystem::createPipelineLayout() {
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
    if (vkCreatePipelineLayout(app_Device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
      throw std::runtime_error("failed to create pipeline layout!");
    }
  }

  void SimpleRenderSystem::createPipeline(VkRenderPass renderPass) {
    assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

    PipelineConfigInfo pipelineConfig{};
    KATEPipeline::defaultPipelineConfigInfo(pipelineConfig);
    pipelineConfig.renderPass = renderPass;
    pipelineConfig.pipelineLayout = pipelineLayout;
    app_Pipeline = std::make_unique<KATEPipeline>(app_Device,"data/shaders/simple_shader.vert.spv","data/shaders/simple_shader.frag.spv",pipelineConfig);
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "Pipeline Succesfully created and Shaders succesfully built from the files"<< "data/shaders/simple_shader.vert.spv" << " and " << "data/shaders/simple_shader.frag.spv" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;

  }

  void SimpleRenderSystem::renderGameObjects(VkCommandBuffer commandBuffer, std::vector<KATEGameObject> &gameObjects, const KATECamera& camera ){

    app_Pipeline->bind(commandBuffer);

    auto projectionView = camera.getProjection()*camera.getView();

    for (auto& obj : gameObjects) {
      //obj.transform.rotation.y = glm::mod(obj.transform.rotation.y + 0.01f, glm::two_pi<float>()); //rotates at y axis
      //obj.transform.rotation.x = glm::mod(obj.transform.rotation.x + 0.01f, glm::two_pi<float>()); //rotates at x axis

      SimplePushConstantData push{}; 
      auto modelMatrix = obj.transform.mat4();
      push.transform = projectionView*modelMatrix;
      push.normalMatrix = obj.transform.normalMatrix();

      vkCmdPushConstants(commandBuffer,pipelineLayout,VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,0,sizeof(SimplePushConstantData),&push);
      obj.model->bind(commandBuffer);
      obj.model->draw(commandBuffer);
    }
  }

}