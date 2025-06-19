#include <kate_model.hpp>
#include <kate_hash.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include <cassert>
#include <cstring>
#include <unordered_map>
#include <iostream>
namespace std{
    template <>
    struct hash<kate::KATEModel::Vertex>{

        size_t operator()(kate::KATEModel::Vertex const &vertex) const {
            size_t seed = 0;
            kate::hashFNV1A(seed,vertex.position,vertex.color,vertex.normal,vertex.uv);
            return seed;

        }
    };
}
namespace kate{ 
    KATEModel::KATEModel( KATEDevice &device,const Builder &builder) : ptrAppDevice{device}{
        createVertexBuffers(builder.vertices);
        createIndexBuffers(builder.indices);
    }
    KATEModel::~KATEModel(){

    }

    std::unique_ptr<KATEModel> KATEModel::createModelFromFile(KATEDevice &device, const std::string& filepath){
        Builder builder{}; 

        builder.loadModel(filepath);

        return std::make_unique<KATEModel>(device,builder);
    }
    
    void KATEModel::createVertexBuffers(const std::vector<Vertex> &vertices){
        vertexCount=static_cast<uint32_t>(vertices.size());
        assert(vertices.size()>=3 && "VERTEX COUNT NEEDS TO BE AT LEAST 3!!!");
        VkDeviceSize bufferSize = sizeof(vertices[0])*vertexCount;
        uint32_t vertexSize = sizeof(vertices[0]);
        KATEBuffer stagingBuffer{
            ptrAppDevice,
            vertexSize,
            vertexCount,
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        };
        stagingBuffer.map();
        stagingBuffer.writeToBuffer((void *)vertices.data(),bufferSize);
        vertexBuffer = std::make_unique<KATEBuffer>(
            ptrAppDevice,
            vertexSize,
            vertexCount,
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        );

        ptrAppDevice.copyBuffer(stagingBuffer.getBuffer(),vertexBuffer->getBuffer(),bufferSize);

        // Vertex buffer created successfully - no assertion needed here since copyBuffer would throw on failure
    }

    void KATEModel::createIndexBuffers(const std::vector<uint32_t> &indices){
        indexCount=static_cast<uint32_t>(indices.size());
        hasIndexBuffer = indexCount>0;
        if(!hasIndexBuffer){
            return;
        }
        
        VkDeviceSize bufferSize = sizeof(indices[0])*indexCount;
        uint32_t indexSize = sizeof(indices[0]);
        KATEBuffer stagingBuffer{
            ptrAppDevice,
            indexSize,
            indexCount,
            VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        };
        stagingBuffer.map();
        stagingBuffer.writeToBuffer((void *)indices.data(),bufferSize);
        indexBuffer = std::make_unique<KATEBuffer>(
            ptrAppDevice,
            indexSize,
            indexCount,
            VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        );

        ptrAppDevice.copyBuffer(stagingBuffer.getBuffer(),indexBuffer->getBuffer(),bufferSize);
    }
    void KATEModel::draw(VkCommandBuffer commandBuffer){
        if(hasIndexBuffer){
            vkCmdDrawIndexed(commandBuffer,indexCount,1,0,0,0);
        }
        else{
            vkCmdDraw(commandBuffer,indexCount,1,0,0);
        }
    }
        
    void KATEModel::bind(VkCommandBuffer commandBuffer){
        VkBuffer buffers[]={vertexBuffer->getBuffer()};
        VkDeviceSize offsets[]={0};
        vkCmdBindVertexBuffers(commandBuffer,0,1,buffers,offsets);

        if(hasIndexBuffer){
            vkCmdBindIndexBuffer(commandBuffer,indexBuffer->getBuffer(),0,VK_INDEX_TYPE_UINT32);
        }
    }
    std::vector<VkVertexInputBindingDescription> KATEModel::Vertex::getBindingDescriptions(){
        std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
        bindingDescriptions[0].binding = 0;
        bindingDescriptions[0].stride = sizeof(Vertex);
        bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return bindingDescriptions;      
        //is the same as:
        //return {{0,sizeof(Vertex),VK_VERTEX_INPUT_RATE_VERTEX}};
    }
    std::vector<VkVertexInputAttributeDescription> KATEModel::Vertex::getAttributesDescriptions(){
        /*
        std::vector<VkVertexInputAttributeDescription> attributesDescriptions(2);
        attributesDescriptions[0].binding = 0;
        attributesDescriptions[0].location = 0;
        attributesDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributesDescriptions[0].offset = offsetof(Vertex,position);

        attributesDescriptions[1].binding = 0;
        attributesDescriptions[1].location = 1;
        attributesDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributesDescriptions[1].offset = offsetof(Vertex,color);
        return attributesDescriptions; */
        return {
            {0,0,VK_FORMAT_R32G32B32_SFLOAT,offsetof(Vertex,position)},
            {1,0,VK_FORMAT_R32G32B32_SFLOAT,offsetof(Vertex,color)},
            {2,0,VK_FORMAT_R32G32B32_SFLOAT,offsetof(Vertex,normal)},
            {3,0,VK_FORMAT_R32G32_SFLOAT,offsetof(Vertex,uv)}
            };
    }
    void KATEModel::Builder::loadModel(const std::string &filepath){
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn,err;

        if(!tinyobj::LoadObj(&attrib,&shapes,&materials,&warn,&err,filepath.c_str())){
            throw std::runtime_error(warn+err);
        }
        vertices.clear();
        indices.clear();

        std::unordered_map<Vertex,uint32_t> uniqueVertices{};

        for(const auto &shape: shapes){
            for(const auto &index : shape.mesh.indices){
                Vertex vertex{};
                if(index.vertex_index>=0){
                    vertex.position = {
                        attrib.vertices[3*index.vertex_index+0],
                        attrib.vertices[3*index.vertex_index+1],
                        attrib.vertices[3*index.vertex_index+2]
                    }; 

                    vertex.color={
                        attrib.colors[3*index.vertex_index+0],
                        attrib.colors[3*index.vertex_index+1],
                        attrib.colors[3*index.vertex_index+2]
                    };

                }
                if(index.normal_index>=0){
                    vertex.normal = {
                        attrib.normals[3*index.normal_index+0],
                        attrib.normals[3*index.normal_index+1],
                        attrib.normals[3*index.normal_index+2]
                    };
                }
                if(index.texcoord_index>=0){
                    vertex.uv = {
                        attrib.texcoords[2*index.texcoord_index+0],
                        attrib.texcoords[2*index.texcoord_index+1] 
                    };
                }
                if(uniqueVertices.count(vertex)==0){
                    uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex);
                }
                indices.push_back(uniqueVertices[vertex]);
            }
        }
    }
    int KATEModel::getnumberOfVertices(){
        return vertexCount;
    }
}