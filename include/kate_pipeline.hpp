#pragma once

#include "kate_device.hpp"
#include <string>
#include <vector>

namespace kate{

    struct PipelineConfigInfo {
        PipelineConfigInfo() = default;
        PipelineConfigInfo(const PipelineConfigInfo&) = delete;
        PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

        std::vector<VkVertexInputBindingDescription> bindingDescriptions{};
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};
        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        VkPipelineViewportStateCreateInfo viewportInfo;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyinfo;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo colorBlendInfo;
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
        std::vector<VkDynamicState> dynamicStateEnables;
        VkPipelineDynamicStateCreateInfo dynamicStateInfo;
        VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
        VkRenderPass renderPass = VK_NULL_HANDLE;
        uint32_t subpass = 0;
    };
    
    class KATEPipeline // This class is used to make a connection between binaries that are compiled in the gpu and the cpu
    {
        public:
            KATEPipeline(KATEDevice& device, const std::string& vertexFilePath,const std::string& fragmentFilePath, const PipelineConfigInfo& pipeline_configuration_info); //constructor
            ~KATEPipeline (); //Destructor
            
            KATEPipeline(const KATEPipeline &) = delete;
            KATEPipeline& operator = (const KATEPipeline&) = delete;
            
            void bind(VkCommandBuffer commandBuffer);
            static void defaultPipelineConfigInfo(PipelineConfigInfo& configI);

        private:
            static std::vector<char> readFile(const std::string& filepath);

            void createGraphicsPipeline(const std::string& vertexFilePath,const std::string& fragmentFilePath, const PipelineConfigInfo& pipeline_configuration_info);

            void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule); //function signature : A function signature (or type signature, or method signature) defines input and output of functions or methods. A signature can include: parameters and their types. a return value and type

            KATEDevice& user_Device;
            VkPipeline graphicsPipeline;
            VkShaderModule vertexShaderModule;
            VkShaderModule fragmentShaderModule;
            
    };
}