#pragma once

#include "kate_device.hpp"

// vulkan headers
#include <vulkan/vulkan.h>
#include <memory>
// std lib headers
#include <string>
#include <vector>

namespace kate {

  class KATESwapChain {
    public:
      static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

      KATESwapChain(KATEDevice &deviceRef, VkExtent2D windowExtent);
      KATESwapChain(KATEDevice &deviceRef, VkExtent2D windowExtent,std::shared_ptr<KATESwapChain> previous);
      ~KATESwapChain();

      KATESwapChain(const KATESwapChain &) = delete;
      KATESwapChain& operator=(const KATESwapChain &) = delete;

      VkFramebuffer getFrameBuffer(int index) { return swapChainFramebuffers[index]; }
      VkRenderPass getRenderPass(){ return renderPass; } //a render pass describes the structure and format of our frame buffer objects and our atachments
      VkImageView getImageView(int index) { return swapChainImageViews[index]; }
      size_t imageCount() { return swapChainImages.size(); }
      VkFormat getSwapChainImageFormat() { return swapChainImageFormat; }
      VkExtent2D getSwapChainExtent() { return swapChainExtent; }
      uint32_t width() { return swapChainExtent.width; }
      uint32_t height() { return swapChainExtent.height; }

      float extentAspectRatio() {
        return static_cast<float>(swapChainExtent.width) / static_cast<float>(swapChainExtent.height);
      }
      VkFormat findDepthFormat();

      VkResult acquireNextImage(uint32_t *imageIndex);
      VkResult submitCommandBuffers(const VkCommandBuffer *buffers, uint32_t *imageIndex);
      bool compareSwapFormats(const KATESwapChain &swapChain) const {
        return swapChain.swapChainDepthFormat == swapChainDepthFormat && swapChain.swapChainImageFormat == swapChainImageFormat;
      }
    private:
      void init();
      void createSwapChain();
      void createImageViews();
      void createDepthResources();
      void createRenderPass();
      void createFramebuffers();
      void createSyncObjects();

      // Helper functions
      VkSurfaceFormatKHR chooseSwapSurfaceFormat(
          const std::vector<VkSurfaceFormatKHR> &availableFormats);
      VkPresentModeKHR chooseSwapPresentMode(
          const std::vector<VkPresentModeKHR> &availablePresentModes);
      VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

      VkFormat swapChainImageFormat;
      VkExtent2D swapChainExtent;

      std::vector<VkFramebuffer> swapChainFramebuffers;
      VkRenderPass renderPass;

      std::vector<VkImage> depthImages;
      std::vector<VkDeviceMemory> depthImageMemorys;
      std::vector<VkImageView> depthImageViews;
      std::vector<VkImage> swapChainImages;
      std::vector<VkImageView> swapChainImageViews;

      KATEDevice &device;
      VkExtent2D windowExtent;

      VkSwapchainKHR swapChain;
      std::shared_ptr<KATESwapChain> oldSwapChain;

      std::vector<VkSemaphore> imageAvailableSemaphores;
      std::vector<VkSemaphore> renderFinishedSemaphores;
      std::vector<VkFence> inFlightFences;
      std::vector<VkFence> imagesInFlight;
      size_t currentFrame = 0;
      VkFormat swapChainDepthFormat;
  };

}