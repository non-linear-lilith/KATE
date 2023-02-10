#pragma once

#include "kate_device.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/glm.hpp>
#include <vector>

namespace kate{
    class KATEModel{
        public:
            struct Vertex{
                glm::vec3 position;
                glm::vec3 color;
                static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
                static std::vector<VkVertexInputAttributeDescription> getAttributesDescriptions();
            };
            
            KATEModel(KATEDevice &device,const std::vector<Vertex> &vertices);
            ~KATEModel();

            KATEModel(const KATEModel&) = delete;
            KATEModel &operator = (const KATEModel&) = delete;

            void bind(VkCommandBuffer commandBuffer);
            void draw(VkCommandBuffer commandBuffer);

        private:
            void createVertexBuffers(const std::vector<Vertex> &vertices);
            KATEDevice &ptrAppDevice;
            VkBuffer vertexBuffer;
            VkDeviceMemory vertexBufferMemory;
            uint32_t vertexCount;
    };
}