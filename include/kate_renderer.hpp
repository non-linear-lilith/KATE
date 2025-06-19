#pragma once
#include "kate_window.hpp"
#include "kate_device.hpp"
#include "kate_swap_chain.hpp"
#include <cassert>
#include <memory>
#include <vector>



namespace kate{
    class KATERenderer 
    {
        public:
            KATERenderer(KATEWindow &window,KATEDevice &device);
            ~KATERenderer();
            KATERenderer(const KATERenderer&)=delete;
            KATERenderer &operator=(const KATERenderer &)=delete;

            bool isFrameInProgress() const {
                return isFrameStarted;
            }  
            VkCommandBuffer getCurrentCommandBuffer()const{
                assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
                return commandBuffers[currentFrameIndex];
            }
            VkCommandBuffer beginFrame();
            void endFrame();
            void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
            void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

            VkRenderPass getSwapChainRenderPass() const { 
                return appSwapChain->getRenderPass(); 
            }
            float getAspectRatio()const{return appSwapChain->extentAspectRatio();}
            int getFrameIndex() const{
                assert(isFrameStarted && "Cannot get frame when frame not in progress");
                return currentFrameIndex;
            }
        private:

            void createCommandBuffers();
            void freeCommandBuffers();
            void drawFrame(); 
            void recreateSwapChain();

            KATEWindow &user_Window;
            KATEDevice &app_Device;
            std::unique_ptr<KATESwapChain> appSwapChain;
            std::vector<VkCommandBuffer> commandBuffers;
            uint32_t currentImageIndex;
            bool isFrameStarted=false;
            int currentFrameIndex = 0 ;
    };
}