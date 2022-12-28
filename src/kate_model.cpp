#include "kate_model.hpp"

#include <cassert>
#include <cstring>
#ifndef ENGINE_DIR
#define ENGINE_DIR "../include"
#endif

namespace kate{ 
    KATEModel::KATEModel( KATEDevice &device,const std::vector<Vertex> &vertices) : ptrAppDevice{device}{
        createVertexBuffers(vertices);
    }
    KATEModel::~KATEModel(){
        vkDestroyBuffer(ptrAppDevice.device(),vertexBuffer,nullptr);
        vkFreeMemory(ptrAppDevice.device(),vertexBufferMemory,nullptr);
    }
    
    void KATEModel::createVertexBuffers(const std::vector<Vertex> &vertices){
        vertexCount=static_cast<uint32_t>(vertices.size());
        assert(vertices.size()>=3 && "VERTEX COUNT NEEDS TO BE AT LEAST 3!!!");
        VkDeviceSize bufferSize = sizeof(vertices[0])*vertexCount;
        ptrAppDevice.createBuffer(bufferSize,VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,vertexBuffer,vertexBufferMemory);
        void *data;
        vkMapMemory(ptrAppDevice.device(),vertexBufferMemory,0,bufferSize,0,&data);
        memcpy(data,vertices.data(),static_cast<size_t>(bufferSize));
        vkUnmapMemory(ptrAppDevice.device(),vertexBufferMemory);
    }
    void KATEModel::draw(VkCommandBuffer commandBuffer){
        vkCmdDraw(commandBuffer,vertexCount,1,0,0);
    }
    void KATEModel::bind(VkCommandBuffer commandBuffer){
        VkBuffer buffers[]={vertexBuffer};
        VkDeviceSize offsets[]={0};
        vkCmdBindVertexBuffers(commandBuffer,0,1,buffers,offsets);
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
        attributesDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
        attributesDescriptions[0].offset = offsetof(Vertex,position);

        attributesDescriptions[1].binding = 0;
        attributesDescriptions[1].location = 1;
        attributesDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributesDescriptions[1].offset = offsetof(Vertex,color);
        return attributesDescriptions; */
        return {
            {0,0,VK_FORMAT_R32G32_SFLOAT,offsetof(Vertex,position)},
            {1,0,VK_FORMAT_R32G32B32_SFLOAT,offsetof(Vertex,color)}
            };
    }
}