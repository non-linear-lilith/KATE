#pragma once

#include "kate_window.hpp"
#include "kate_renderer.hpp" 
#include "kate_game_object.hpp" 
#include "kate_imgui_manager.hpp"
#include <kate_descriptors.hpp>
#include <kate_device.hpp>

#include <memory>
#include <vector>
#include <deque>
#include <chrono>
#include <limits>
#include <cmath>
#include <algorithm>
#include <map>



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
            void updateFPS(float frameTime);
            void renderFPSWindow();
            void calculateGaussianDistribution();
            void calculateAdvancedStatistics();
            float gaussianFunction(float x, float mean, float stddev);
            float calculateMedian(std::vector<float>& data);
            float calculateMode(const std::vector<float>& data);
            float calculatePercentile(std::vector<float>& data, float percentile);
            float calculateSkewness(const std::vector<float>& data, float mean, float stddev);
            float calculateKurtosis(const std::vector<float>& data, float mean, float stddev);

            struct FPSData {
                float fps;
                std::chrono::steady_clock::time_point timestamp;
            };

            std::string AppName = "(KAT)herine (E)ngine testing and debbuging";

            KATEWindow user_Window{WIDTH,HEIGHT,AppName};
            KATEDevice app_Device{user_Window};
            
            
            std::unique_ptr<KATERenderer> appRenderer = std::make_unique<KATERenderer>(user_Window, app_Device);
            std::unique_ptr<KATEImGuiManager> imguiManager  = std::make_unique<KATEImGuiManager>(user_Window, app_Device, appRenderer->getSwapChainRenderPass(),KATESwapChain::MAX_FRAMES_IN_FLIGHT);

            std::unique_ptr<KATEDescriptorPool> globalPool{};
            KATEGameObject::Map gameObjects;
            
            // FPS tracking
            std::deque<FPSData> fpsHistory;
            static constexpr float FPS_WINDOW_SECONDS = 20.0f;
            float currentFPS = 0.0f;
            float averageFPS = 0.0f;
            float minFPS = std::numeric_limits<float>::max();
            float maxFPS = 0.0f;
            
            // Last 20 frames tracking for Gaussian distribution
            std::deque<float> last20Frames;
            static constexpr int MAX_FRAME_SAMPLES = 20;
            float frameMean = 0.0f;
            float frameStdDev = 0.0f;
            std::vector<float> gaussianValues;
            
            // Advanced statistics
            float frameMedian = 0.0f;
            float frameMode = 0.0f;
            float frame25thPercentile = 0.0f;
            float frame75thPercentile = 0.0f;
            float frame95thPercentile = 0.0f;
            float frameSkewness = 0.0f;
            float frameKurtosis = 0.0f;
            
    };
}