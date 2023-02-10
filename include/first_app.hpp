#pragma once
#include "kate_window.hpp"
#include "kate_device.hpp"
#include "kate_renderer.hpp"
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

            std::string AppName = "(KAT)herine (E)ngine testing and debbuging";

            KATEWindow user_Window{WIDTH,HEIGHT,AppName};
            KATEDevice app_Device{user_Window};
            
            
            KATERenderer appRenderer{user_Window,app_Device};
            std::vector<KATEGameObject> gameObjects;
    };
}