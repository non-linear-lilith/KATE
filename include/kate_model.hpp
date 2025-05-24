#pragma once

#include "kate_device.hpp"
#include <kate_buffer.hpp>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <memory>
#include <glm/glm.hpp>
#include <vector>

namespace kate{
    class KATEModel{
        public:
            struct Vertex{
                glm::vec3 position{};
                glm::vec3 color{};
                glm::vec3 normal{};
                glm::vec2 uv{};

                static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
                static std::vector<VkVertexInputAttributeDescription> getAttributesDescriptions();
                bool operator==(const Vertex &other) const {
                    return position == other.position && color == other.color && normal == other.normal && uv == other.uv;
                }
            };
            /**
             * Is used to store the data of a .obj file in the memory as a object to load a model that will be used to send the data to the device. 
             * It is used TinyOBJLoader to load the model.
             */
            struct Builder{
                std::vector<Vertex> vertices{}; //Is a vector of every vertex that the builder will use to create a model
                std::vector<uint32_t> indices{}; //Is a vector with indices that points the joints of the vertices in the model
                /**
                 * Load a .obj model that is located in a the "data/models/" folder into the memory. 
                 * 
                 * @param filepath Is the file address that is located in "data/models/"
                 * 
                 */
                void loadModel(const std::string &filepath);
            };

            KATEModel(KATEDevice &device,const Builder &builder);
            ~KATEModel();

            KATEModel(const KATEModel&) = delete;
            KATEModel &operator = (const KATEModel&) = delete;
            /**
             * Creates a model into the gpu with a device and a filepath to a .obj model using a Builder
             * @param device Is a memory address that points to the device (GPU)
             * @param filepath Is a filepath to a .obj file that will rendered
            */
            static std::unique_ptr<KATEModel> createModelFromFile(KATEDevice &device, const std::string& filepath); 
            void bind(VkCommandBuffer commandBuffer);
            /**
             * If a index buffer is found, inserts a vulkan Command buffer into the gpu, draws a image into the GLFW from the GPU
            */
            void draw(VkCommandBuffer commandBuffer);
            int getnumberOfVertices();
        private:
            void createVertexBuffers(const std::vector<Vertex> &vertices);
            void createIndexBuffers(const std::vector<uint32_t> &indices);
            KATEDevice &ptrAppDevice;
            
            std::unique_ptr<KATEBuffer> vertexBuffer;
            uint32_t vertexCount;

            bool hasIndexBuffer = false;
            std::unique_ptr<KATEBuffer> indexBuffer;
            uint32_t indexCount;
    };
}