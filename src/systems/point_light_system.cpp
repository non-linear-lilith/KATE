#include <systems/point_light_system.hpp>

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
    glm::mat4 modelMatrix{1.f};
    glm::mat4 normalMatrix{1.f};
};

PointLightSystem::PointLightSystem(
    KATEDevice& device, 
    VkRenderPass renderPass,
    VkDescriptorSetLayout globalSetLayout
) : SRS_Device{device} {
    createPipelineLayout(globalSetLayout);
    createPipeline(renderPass);

}
PointLightSystem::~PointLightSystem() {
    vkDestroyPipelineLayout(SRS_Device.device(), pipelineLayout, nullptr);
}

void PointLightSystem::createPipelineLayout(VkDescriptorSetLayout globalSetLayout) {
    // VkPushConstantRange pushConstantRange{};
    // pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    // pushConstantRange.offset = 0;
    // pushConstantRange.size = sizeof(SimplePushConstantData);

    std::vector<VkDescriptorSetLayout> descriptorSetLayouts{globalSetLayout};

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
    pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;

    if (vkCreatePipelineLayout(SRS_Device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }
}

void PointLightSystem::createPipeline(VkRenderPass renderPass) {
    assert(pipelineLayout != VK_NULL_HANDLE && "Cannot create pipeline before pipeline layout");

    PipelineConfigInfo pipelineConfig{};
    KATEPipeline::defaultPipelineConfigInfo(pipelineConfig);
    
    // Point light shader doesn't use vertex inputs - clear vertex input state
    pipelineConfig.bindingDescriptions.clear();
    pipelineConfig.attributeDescriptions.clear();
    pipelineConfig.vertexInputInfo.vertexAttributeDescriptionCount = 0;
    pipelineConfig.vertexInputInfo.pVertexAttributeDescriptions = nullptr;
    pipelineConfig.vertexInputInfo.vertexBindingDescriptionCount = 0;
    pipelineConfig.vertexInputInfo.pVertexBindingDescriptions = nullptr;
    
    pipelineConfig.renderPass = renderPass;
    pipelineConfig.pipelineLayout = pipelineLayout;
    app_Pipeline = std::make_unique<KATEPipeline>(
        SRS_Device,
        "data/shaders/point_light.vert.spv",
        "data/shaders/point_light.frag.spv",
        pipelineConfig
    );
    std::cout   << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout   << "Pipeline Succesfully created and Shaders succesfully built from the files"
                << "data/shaders/point_light.vert.spv"
                << " and "
                << "data/shaders/point_light.frag.spv"
                << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
}

void PointLightSystem::render(FrameInfo& frameInfo) {
    app_Pipeline->bind(frameInfo.commandBuffer);

    vkCmdBindDescriptorSets(
        frameInfo.commandBuffer,
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        pipelineLayout,
        0,
        1,
        &frameInfo.globalDescriptorSet,
        0,
        nullptr
    );
    vkCmdDraw(
        frameInfo.commandBuffer,
        6, // Six vertices for a cube
        1, // One instance
        0, // First vertex
        0  // First instance
    );

    // for (auto& kv : frameInfo.gameObjects) {
    //     auto& obj = kv.second;
    //     if (obj.model == nullptr) continue;

    //     SimplePushConstantData push{};
    //     push.modelMatrix = obj.transform.mat4();
    //     push.normalMatrix = obj.transform.normalMatrix();

    //     vkCmdPushConstants(
    //         frameInfo.commandBuffer,
    //         pipelineLayout,
    //         VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
    //         0,
    //         sizeof(SimplePushConstantData),
    //         &push
    //     );
    //     obj.model->bind(frameInfo.commandBuffer);
    //     obj.model->draw(frameInfo.commandBuffer);
    // }
}

}