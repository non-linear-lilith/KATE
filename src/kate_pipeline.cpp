#include "kate_pipeline.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cassert>
#ifndef ENGINE_DIR
#define ENGINE_DIR "../include"
#endif

namespace kate{

    KATEPipeline::KATEPipeline(KATEDevice& device, const std::string& vertexFilePath,const std::string& fragmentFilePath, const PipelineConfigInfo& pipeline_configuration_info): user_Device{device} {
        createGraphicPipeline(vertexFilePath,fragmentFilePath,pipeline_configuration_info);
    }
    KATEPipeline::~KATEPipeline(){
        vkDestroyShaderModule(user_Device.device(),vertexShaderModule,nullptr);
        vkDestroyShaderModule(user_Device.device(),fragmentShaderModule,nullptr);
        vkDestroyPipeline(user_Device.device(),graphicsPipeline,nullptr);
    }

    std::vector<char> KATEPipeline::readFile(const std::string& filepath){
        std::ifstream file{filepath,std::ios::ate | std::ios::binary};
        if(!file.is_open()){
            
            throw std::runtime_error("\x1B[31mFATAL ERROR Failed to open file "+ filepath+"\033[0m");
        }
        size_t fileSize = static_cast<size_t>(file.tellg());
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(),fileSize);

        file.close();
        return buffer;
    }

    void KATEPipeline::createGraphicPipeline(const std::string& vertex_Filepath, const std::string& fragment_Filepath,const PipelineConfigInfo& pipeline_configuration_info){
        assert(pipeline_configuration_info.pipelineLayout!=VK_NULL_HANDLE && "Cannot create graphics pipeline: no pipelineLayout provided in configInfo UnU.");
        assert(pipeline_configuration_info.renderPass!=VK_NULL_HANDLE && "Cannot create graphics pipeline: no renderPass provided in configInfo UNU.");
        auto vertex_Code = readFile(vertex_Filepath);
        auto fragment_Code = readFile(fragment_Filepath);

        createShaderModule(vertex_Code, &vertexShaderModule);
        createShaderModule(fragment_Code, &fragmentShaderModule);


        VkPipelineShaderStageCreateInfo shaderStages[2];

        //Pipeline Vertex info 
        shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
        shaderStages[0].module = vertexShaderModule;
        shaderStages[0].pName = "Main";
        shaderStages[0].flags = 0;
        shaderStages[0].pNext = nullptr;
        shaderStages[0].pSpecializationInfo = nullptr;

        //Pipeline Fragment info
        shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        shaderStages[1].module = fragmentShaderModule;
        shaderStages[1].pName = "Main";
        shaderStages[1].flags = 0;
        shaderStages[1].pNext = nullptr;
        shaderStages[1].pSpecializationInfo = nullptr;

        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexAttributeDescriptionCount = 0;
        vertexInputInfo.vertexBindingDescriptionCount = 0;
        vertexInputInfo.pVertexAttributeDescriptions = nullptr;
        vertexInputInfo.pVertexBindingDescriptions = nullptr;

        VkPipelineViewportStateCreateInfo viewportInfo{};
        viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportInfo.viewportCount = 1;
        viewportInfo.pViewports = &pipeline_configuration_info.viewport;
        viewportInfo.scissorCount = 1;
        viewportInfo.pScissors = &pipeline_configuration_info.scissor;

        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages; 
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &pipeline_configuration_info.inputAssemblyinfo;
        pipelineInfo.pViewportState = &viewportInfo;
        pipelineInfo.pRasterizationState = &pipeline_configuration_info.rasterizationInfo;
        pipelineInfo.pMultisampleState = &pipeline_configuration_info.multisampleInfo;

        pipelineInfo.pColorBlendState = &pipeline_configuration_info.colorBlendInfo;
        pipelineInfo.pDynamicState = nullptr;

        pipelineInfo.layout = pipeline_configuration_info.pipelineLayout;
        pipelineInfo.renderPass = pipeline_configuration_info.renderPass;
        pipelineInfo.subpass = pipeline_configuration_info.subpass;

        pipelineInfo.basePipelineIndex = -1;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

        if(vkCreateGraphicsPipelines(user_Device.device(),VK_NULL_HANDLE,1,&pipelineInfo,nullptr,&graphicsPipeline) != VK_SUCCESS ){
            throw std::runtime_error(" \x1B[31m FATAL ERROR: Failed to create graphics pipeline UnU\033[0m");
        }

    }

    void KATEPipeline::createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule){
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
        if(vkCreateShaderModule(user_Device.device(),&createInfo,nullptr,shaderModule)!= VK_SUCCESS){
            throw std::runtime_error(" \x1B[31m----------FATAL ERROR----------\n Failed to create shader Module\033[0m ");
        }
    }

    PipelineConfigInfo KATEPipeline::defaultPipelineConfigInfo(uint32_t width, uint32_t height){
        PipelineConfigInfo configInfo{};
        //Assembly Info

        configInfo.inputAssemblyinfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        configInfo.inputAssemblyinfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        configInfo.inputAssemblyinfo.primitiveRestartEnable = VK_FALSE; 
        //Viewport

        configInfo.viewport.x = 0.0f;
        configInfo.viewport.y = 0.0f;
        configInfo.viewport.width = static_cast<float>(width);
        configInfo.viewport.height = static_cast<float>(height);
        configInfo.viewport.minDepth = 0.0f;
        configInfo.viewport.maxDepth = 1.0f;
        //Viewport Info



        //Scissor
        configInfo.scissor.offset = {0, 0};
        configInfo.scissor.extent = {width, height};

        //Rasterization Info
        configInfo.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        configInfo.rasterizationInfo.depthClampEnable = VK_FALSE;
        configInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
        configInfo.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
        configInfo.rasterizationInfo.lineWidth = 1.0f;
        configInfo.rasterizationInfo.cullMode = VK_CULL_MODE_NONE;
        configInfo.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
        configInfo.rasterizationInfo.depthBiasEnable = VK_FALSE;
        configInfo.rasterizationInfo.depthBiasConstantFactor = 0.0f;  // Optional
        configInfo.rasterizationInfo.depthBiasClamp = 0.0f;           // Optional
        configInfo.rasterizationInfo.depthBiasSlopeFactor = 0.0f;     // Optional
        
        //Multisample Info
        configInfo.multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        configInfo.multisampleInfo.sampleShadingEnable = VK_FALSE;
        configInfo.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        configInfo.multisampleInfo.minSampleShading = 1.0f;           // Optional
        configInfo.multisampleInfo.pSampleMask = nullptr;             // Optional
        configInfo.multisampleInfo.alphaToCoverageEnable = VK_FALSE;  // Optional
        configInfo.multisampleInfo.alphaToOneEnable = VK_FALSE;       // Optional

        //colorBlendAttachment
        configInfo.colorBlendAttachment.colorWriteMask =VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        configInfo.colorBlendAttachment.blendEnable = VK_FALSE;
        configInfo.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
        configInfo.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
        configInfo.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;              // Optional
        configInfo.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
        configInfo.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
        configInfo.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;              // Optional

        //ColorBlend Info
        configInfo.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        configInfo.colorBlendInfo.logicOpEnable = VK_FALSE;
        configInfo.colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;  // Optional
        configInfo.colorBlendInfo.attachmentCount = 1;
        configInfo.colorBlendInfo.pAttachments = &configInfo.colorBlendAttachment;
        configInfo.colorBlendInfo.blendConstants[0] = 0.0f;  // Optional
        configInfo.colorBlendInfo.blendConstants[1] = 0.0f;  // Optional
        configInfo.colorBlendInfo.blendConstants[2] = 0.0f;  // Optional
        configInfo.colorBlendInfo.blendConstants[3] = 0.0f;  // Optional

        //Depth Stencil Info
        configInfo.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        configInfo.depthStencilInfo.depthTestEnable = VK_TRUE;
        configInfo.depthStencilInfo.depthWriteEnable = VK_TRUE;
        configInfo.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
        configInfo.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
        configInfo.depthStencilInfo.minDepthBounds = 0.0f;  // Optional
        configInfo.depthStencilInfo.maxDepthBounds = 1.0f;  // Optional
        configInfo.depthStencilInfo.stencilTestEnable = VK_FALSE;
        configInfo.depthStencilInfo.front = {};  // Optional
        configInfo.depthStencilInfo.back = {};   // Optional

        return configInfo;
    }

}