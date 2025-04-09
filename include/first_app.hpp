#pragma once
#include "kate_window.hpp"
#include "kate_device.hpp"
#include "kate_renderer.hpp"
#include "kate_game_object.hpp"
#include "kate_imgui_manager.hpp"

#include <memory>
#include <vector>



namespace kate{
    class FirstApp 
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


            std::string AppName = "(KAT)herine (E)ngine testing and debbuging";

            KATEWindow user_Window{WIDTH,HEIGHT,AppName};
            KATEDevice app_Device{user_Window};
            
            
            std::unique_ptr<KATERenderer> appRenderer = std::make_unique<KATERenderer>(user_Window, app_Device);
            std::unique_ptr<KATEImGuiManager> imguiManager  = std::make_unique<KATEImGuiManager>(user_Window, app_Device, appRenderer->getSwapChainRenderPass(),KATESwapChain::MAX_FRAMES_IN_FLIGHT);

            std::vector<KATEGameObject> gameObjects;
            
    };
}