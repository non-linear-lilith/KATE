#pragma once
#include "kate_camera.hpp"

#include <vulkan/vulkan.h>

namespace kate {
    struct FrameInfo{
        int frameIndex;
        float frameTime;
        VkCommandBuffer commandBuffer;
        KATECamera& camera;
    };
} // namespace kate
