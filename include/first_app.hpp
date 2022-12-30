#pragma once
#include "kate_window.hpp"
#include "kate_pipeline.hpp"
#include "kate_device.hpp"
#include "kate_swap_chain.hpp"
#include "kate_game_object.hpp"

#include <memory>
#include <vector>



namespace kate{
    class FirstApp //https://www.youtube.com/watch?v=lr93-_cC8v4&list=PL8327DO66nu9qYVKLDmdLW_84-yE4auCR&index=2
    {
        public:
            FirstApp();
            ~FirstApp();
            FirstApp(const FirstApp&)=delete;
            FirstApp &operator=(const FirstApp &)=delete;
            void run();

            static constexpr int WIDTH = 800;
            static constexpr int HEIGHT = 600;

        private:
            void loadGameObjects(); 
            void createPipelineLayout();
            void createPipeline();
            void createCommandBuffers();
            void freeCommandBuffers();
            void drawFrame();
            void recreateSwapChain();
            void recordCommandBuffer(int imageIndex);
            void renderGameObjects(VkCommandBuffer VkCommandBuffer);

            const std::string AppName = "KATherine Engine TESTING and DEBUGGING";
            KATEWindow user_Window{WIDTH,HEIGHT,AppName};
            KATEDevice app_Device{user_Window};
            std::unique_ptr<KATESwapChain> appSwapChain;
            std::unique_ptr<KATEPipeline> appPipeline;
            VkPipelineLayout pipelineLayout;
            std::vector<VkCommandBuffer> commandBuffers;
            std::vector<KATEGameObject> gameObjects;
    };
}