#pragma once

#include "kate_device.hpp"
#include "kate_game_object.hpp"
#include "kate_pipeline.hpp"
#include "kate_camera.hpp"
#include "kate_frame_info.hpp"
// std
#include <memory>
#include <vector>

namespace kate {
class SimpleRenderSystem {
  public:
    SimpleRenderSystem(KATEDevice &device, VkRenderPass renderPass,VkDescriptorSetLayout globalSetLayout);
    ~SimpleRenderSystem();

    SimpleRenderSystem(const SimpleRenderSystem &) = delete;
    SimpleRenderSystem &operator=(const SimpleRenderSystem &) = delete;
    void renderGameObjects(FrameInfo& frameInfo);

  private:
    void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
    void createPipeline(VkRenderPass renderPass);

    KATEDevice &SRS_Device;

    std::unique_ptr<KATEPipeline> app_Pipeline ; 
    VkPipelineLayout pipelineLayout;
  };
}