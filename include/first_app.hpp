#pragma once

#include "kate_window.hpp"
#include "kate_pipeline.hpp"
#include "kate_device.hpp"

namespace kate{
    class FirstApp //https://www.youtube.com/watch?v=lr93-_cC8v4&list=PL8327DO66nu9qYVKLDmdLW_84-yE4auCR&index=2
    {
        public:
            static constexpr int WIDTH = 800;
            static constexpr int HEIGHT = 600;
            void run();
        private:
            const std::string AppName = "Visual Novel";
            KATEWindow user_Window{WIDTH,HEIGHT,AppName};
            KATEDevice app_Device{user_Window};
            KATEPipeline app_Pipeline{app_Device,"shaders/simple_shader.vert.spv","shaders/simple_shader.frag.spv",KATEPipeline::defaultPipelineConfigInfo(WIDTH,HEIGHT)};
    };
}