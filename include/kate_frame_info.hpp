#pragma once
#include "kate_camera.hpp"
#include <kate_game_object.hpp>
#include <vulkan/vulkan.h>

namespace kate {
    struct FrameInfo{
        int frameIndex;
        float frameTime;
        VkCommandBuffer commandBuffer ;
        KATECamera& camera ;
        VkDescriptorSet globalDescriptorSet ;
        KATEGameObject::Map &gameObjects; // Map of game objects to be rendered
    };
} // namespace kate
