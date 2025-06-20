#include <systems/simple_render_system.hpp>

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

SimpleRenderSystem::SimpleRenderSystem(
    KATEDevice& device, 
    VkRenderPass renderPass,
    VkDescriptorSetLayout globalSetLayout
) : SRS_Device{device} {
    createPipelineLayout(globalSetLayout);
    createPipeline(renderPass);

}
SimpleRenderSystem::~SimpleRenderSystem() {
    vkDestroyPipelineLayout(SRS_Device.device(), pipelineLayout, nullptr);
}

void SimpleRenderSystem::createPipelineLayout(VkDescriptorSetLayout globalSetLayout) {
    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(SimplePushConstantData);

    std::vector<VkDescriptorSetLayout> descriptorSetLayouts{globalSetLayout};

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
    pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

    if (vkCreatePipelineLayout(SRS_Device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }
}

void SimpleRenderSystem::createPipeline(VkRenderPass renderPass) {
    assert(pipelineLayout != VK_NULL_HANDLE && "Cannot create pipeline before pipeline layout");

    PipelineConfigInfo pipelineConfig{};
    KATEPipeline::defaultPipelineConfigInfo(pipelineConfig);
    pipelineConfig.renderPass = renderPass;
    pipelineConfig.pipelineLayout = pipelineLayout;
    app_Pipeline = std::make_unique<KATEPipeline>(
        SRS_Device,
        "data/shaders/simple_shader.vert.spv",
        "data/shaders/simple_shader.frag.spv",
        pipelineConfig
    );
    std::cout   << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout   << "Pipeline Succesfully created and Shaders succesfully built from the files"
                << "data/shaders/simple_shader.vert.spv"
                << " and "
                << "data/shaders/simple_shader.frag.spv"
                << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
}

void SimpleRenderSystem::renderGameObjects(FrameInfo& frameInfo) {
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

    for (auto& kv : frameInfo.gameObjects) {
        auto& obj = kv.second;
        if (obj.model == nullptr) continue;

        SimplePushConstantData push{};
        push.modelMatrix = obj.transform.mat4();
        push.normalMatrix = obj.transform.normalMatrix();

        vkCmdPushConstants(
            frameInfo.commandBuffer,
            pipelineLayout,
            VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
            0,
            sizeof(SimplePushConstantData),
            &push
        );
        obj.model->bind(frameInfo.commandBuffer);
        obj.model->draw(frameInfo.commandBuffer);
    }
}

}