#pragma once

#include "kate_device.hpp"
#include "kate_game_object.hpp"
#include "kate_pipeline.hpp"

// std
#include <memory>
#include <vector>

namespace kate {
class SimpleRenderSystem {
 public:
  SimpleRenderSystem(KATEDevice &device, VkRenderPass renderPass);
  ~SimpleRenderSystem();

  SimpleRenderSystem(const SimpleRenderSystem &) = delete;
  SimpleRenderSystem &operator=(const SimpleRenderSystem &) = delete;

  void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<KATEGameObject> &gameObjects);

 private:
  void createPipelineLayout();
  void createPipeline(VkRenderPass renderPass);

  KATEDevice &app_Device;

  std::unique_ptr<KATEPipeline> app_Pipeline;
  VkPipelineLayout pipelineLayout;
};
}