#pragma once

#include <kate_device.hpp>
#include <kate_game_object.hpp>
#include <kate_pipeline.hpp>
#include <kate_camera.hpp>
#include <kate_frame_info.hpp>
// std
#include <memory>
#include <vector>

namespace kate {
class PointLightSystem {
  public:
    PointLightSystem(KATEDevice &device, VkRenderPass renderPass,VkDescriptorSetLayout globalSetLayout);
    ~PointLightSystem();

    PointLightSystem(const PointLightSystem &) = delete;
    PointLightSystem &operator=(const PointLightSystem &) = delete;
    void render(FrameInfo& frameInfo);

  private:
    void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
    void createPipeline(VkRenderPass renderPass);

    KATEDevice &SRS_Device;

    std::unique_ptr<KATEPipeline> app_Pipeline ; 
    VkPipelineLayout pipelineLayout;
  };
}