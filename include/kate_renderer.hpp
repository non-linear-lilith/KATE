#pragma once
#include "kate_window.hpp"
#include "kate_device.hpp"
#include "kate_swap_chain.hpp"

#include <memory>
#include <vector>



namespace kate{
    class KATERenderer //https://www.youtube.com/watch?v=lr93-_cC8v4&list=PL8327DO66nu9qYVKLDmdLW_84-yE4auCR&index=2
    {
        public:
            KATERenderer(KATEWindow &window,KATEDevice &device);
            ~KATERenderer();
            KATERenderer(const KATERenderer&)=delete;
            KATERenderer &operator=(const KATERenderer &)=delete;

        private:

            void createCommandBuffers();
            void freeCommandBuffers();
            void drawFrame();
            void recreateSwapChain();

            KATEWindow user_Window;
            KATEDevice app_Device{user_Window};
            std::unique_ptr<KATESwapChain> appSwapChain;
            std::vector<VkCommandBuffer> commandBuffers;
    };
}