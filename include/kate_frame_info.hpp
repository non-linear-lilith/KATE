#pragma once
#include "kate_camera.hpp"

#include <vulkan/vulkan.h>

namespace kate {
    struct FrameInfo{
        int frameIndex;
        float frameTime;
        VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
        KATECamera& camera ;
        VkDescriptorSet globalDescriptorSet = VK_NULL_HANDLE;
    };
} // namespace kate
