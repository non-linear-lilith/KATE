
#include <kate_camera.hpp>
#include <first_app.hpp>
#include <systems/simple_render_system.hpp>
#include <systems/point_light_system.hpp>

#include "input/keyboard_input.hpp"
#include <kate_buffer.hpp>

//glm libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
    

#include <glm/gtc/constants.hpp>
#include <glm/glm.hpp>
//std libs
#define _USE_MATH_DEFINES
#include <cmath>

#include <array>
#include <chrono>
#include <iostream>
#include <stdexcept>
#include <string.h>

const float MAX_FRAME_TIME = 0.05f;
const float pi2 = 6.28f;

namespace kate{
    struct GlobalUbo{
        glm::mat4 projection{1.f};
        glm::mat4 view{1.f};
        glm::vec4 ambientLightColor{1.f,1.f,1.f,.02f}; // Ambient light color, white in this case {r,g,b,intensity}
        glm::vec3 lightPositition{-1.f}; // Light position in world space {x,y,z}
        alignas(16) glm::vec4 lightColor{1.f,1.f,1.f,16.f}; // Light color {r,g,b,intensity}
    };

    FirstApp::FirstApp(){
        globalPool = KATEDescriptorPool::Builder(app_Device)
        .setMaxSets(KATESwapChain::MAX_FRAMES_IN_FLIGHT)
        .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, KATESwapChain::MAX_FRAMES_IN_FLIGHT)
        .build();
        loadGameObjects();
    }
    FirstApp::~FirstApp(){
    }
    static double xpos, ypos;

    void FirstApp::run() {
        std::vector<std::unique_ptr<KATEBuffer>> uboBuffers(KATESwapChain::MAX_FRAMES_IN_FLIGHT);

        for (int i = 0; i < uboBuffers.size(); i++) {
            uboBuffers[i] = std::make_unique<KATEBuffer>(
                app_Device,
                sizeof(GlobalUbo),
                1,
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
                uboBuffers[i]->map();
        }
        /** *
         * Create a descriptor set layout for the global ubo 
         * with a size of 1 and a type of uniform buffer. 
         * Uniform buffers are used to pass data to the shaders. 
         * 
         * */
        auto globalSetLayout = 
            KATEDescriptorSetLayout::Builder(app_Device)
                .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS) // Binding 0 for uniform buffer
                .build();

        /** *
         * Create a vector of descriptor sets for the global ubo
         * with a size of KATESwapChain::MAX_FRAMES_IN_FLIGHT
         * and a type of uniform buffer.
         * The descriptor sets are used to pass data to the shaders.
         * 
         * */
        std::vector<VkDescriptorSet> globalDescriptorSets(KATESwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < globalDescriptorSets.size(); i++) {
            auto bufferInfo = uboBuffers[i]->descriptorInfo();
            KATEDescriptorWriter(*globalSetLayout, *globalPool)
                .writeBuffer(0, &bufferInfo)
                .build(globalDescriptorSets[i]);
        }

        static float accumulated_frame_time = 0; //Time of frames accumulated per second to be used for the frame counter
        static uint16_t frame_counter = 0;// Number of frames that are displayed per second
        
        SimpleRenderSystem simpleRenderSystem{
            app_Device, 
            appRenderer->getSwapChainRenderPass(),
            globalSetLayout->getDescriptorSetLayout()}; // Create the render system

        PointLightSystem pointLightSystem{
            app_Device, 
            appRenderer->getSwapChainRenderPass(),
            globalSetLayout->getDescriptorSetLayout()}; // Create the render system

        KATECamera camera{};
        //camera.setViewTarget(glm::vec3(-1.f,-1.f,1.f),glm::vec3(0.f,0.f,2.5f)); //set camera angle and position}
        auto viewerObject = KATEGameObject::createGameObject();
        viewerObject.transform.translation.z = -2.5f;

        KeyboardInput cameraController;
        //glfwSetInputMode(user_Window.getGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN); //HIDE CURSOR  
        
        auto currentTime{std::chrono::high_resolution_clock::now()};

        static float rad = 0.f;

        while (!user_Window.shouldClose()) {
            glfwPollEvents();    // Poll for events (keyboard, mouse, etc.)

            imguiManager->newFrame(); // Start a new ImGui frame

            // Define your ImGui UI elements
            

            auto newTime{std::chrono::high_resolution_clock::now()}; // Get the current time
            float frameTime = std::chrono::duration<float,std::chrono::seconds::period>(newTime-currentTime).count(); // Calculate the time since the last frame
            currentTime = newTime;
            frameTime = glm::min(frameTime,MAX_FRAME_TIME);
            glfwGetCursorPos(user_Window.getGLFWWindow(),&xpos,&ypos); 

            // Update FPS tracking
            updateFPS(frameTime);

            // Object Data Window
            ImGui::Begin("Object Data");
            ImGui::Text("Rat Data:");
            ImGui::Text("Position: (%.3f, %.3f, %.3f)", 
                gameObjects.begin()->second.transform.translation.x,
                gameObjects.begin()->second.transform.translation.y,
                gameObjects.begin()->second.transform.translation.z);
            ImGui::Text("Rotation Angle: %.3f", rad);
            ImGui::Text("Scale: (%.3f, %.3f, %.3f)", 
                gameObjects.begin()->second.transform.scale.x,
                gameObjects.begin()->second.transform.scale.y,
                gameObjects.begin()->second.transform.scale.z);
            
            ImGui::Separator();
            
            ImGui::Text("Floor Data:");
            auto floorIt = std::next(gameObjects.begin());
            if (floorIt != gameObjects.end()) {
                ImGui::Text("Position: (%.3f, %.3f, %.3f)", 
                    floorIt->second.transform.translation.x,
                    floorIt->second.transform.translation.y,
                    floorIt->second.transform.translation.z);
                ImGui::Text("Scale: (%.3f, %.3f, %.3f)", 
                    floorIt->second.transform.scale.x,
                    floorIt->second.transform.scale.y,
                    floorIt->second.transform.scale.z);
            }
            ImGui::End();

            // Mouse and Camera Data Window
            ImGui::Begin("Mouse & Camera Data");
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Mouse Position: ");
            ImGui::Text("X: %.3f", xpos);
            ImGui::Text("Y: %.3f", ypos);
            
            ImGui::Separator();
            
            ImGui::Text("Camera Position: ");
            ImGui::Text("X: %.3f", viewerObject.transform.translation.x);
            ImGui::Text("Y: %.3f", viewerObject.transform.translation.y);
            ImGui::Text("Z: %.3f", viewerObject.transform.translation.z);
            ImGui::Text("Camera Rotation: ");
            ImGui::Text("X: %.3f", viewerObject.transform.rotation.x);
            ImGui::Text("Y: %.3f", viewerObject.transform.rotation.y);
            ImGui::End();

            // Render the new FPS statistics window
            renderFPSWindow();
            
            
            cameraController.moveInPlaneXZ(user_Window.getGLFWWindow(),frameTime,viewerObject);
            cameraController.movepos(user_Window.getGLFWWindow(),frameTime,gameObjects.at(0)); // Move the rat object with the keyboard input
            camera.setViewYXZ2(viewerObject.transform.translation,viewerObject.transform.rotation);
            float aspect = appRenderer->getAspectRatio();
            //camera.setOrthographicProjection(-aspect,aspect,-1,1,-1,1); Orthographic projection
            camera.setPerspectiveProjection(glm::radians(50.f),aspect,0.1f,1000.f); //set camera in perspective projections
            if (auto commandBuffer = appRenderer->beginFrame()) {
                int frameIndex = appRenderer->getFrameIndex();
                FrameInfo frameInfo{
                    frameIndex,
                    frameTime,
                    commandBuffer,
                    camera,
                    globalDescriptorSets[frameIndex],
                    gameObjects 
                };
                GlobalUbo ubo = {};
                ubo.projection = camera.getProjection();
                ubo.view  =  camera.getView();
                uboBuffers[frameIndex]->writeToBuffer(&ubo, sizeof(ubo));//test
                uboBuffers[frameIndex]->flush();

                appRenderer->beginSwapChainRenderPass(commandBuffer); // begin render pass

                simpleRenderSystem.renderGameObjects(frameInfo); // render game objects
                pointLightSystem.render(frameInfo); // render point lights
                imguiManager->render(commandBuffer); // render imgui

                appRenderer->endSwapChainRenderPass(commandBuffer); // end render pass

                appRenderer->endFrame(); // end frame
            }
            // INCREIVLE, UNA RATA QUE GIRA EN CIRCULOS OMG!!1 
            gameObjects.at(0).transform.rotation = {0.f,glm::pi<float>()/2.f+rad,glm::pi<float>()};
            float frecuency = 10.0f; // Frequency of the rotation in radians per second
            rad += frecuency * frameTime; // Scale increment by frame time
            if (rad >= pi2) {
                rad -= pi2;
            }
        }
        vkDeviceWaitIdle(app_Device.device());
    }



    
    /**
     * @brief Load every game object to be rendered using the paths to the models into the engine
     * 
     * Load the game objects to be rendered
     * 
     * @return void
     * 
     */
    void FirstApp::loadGameObjects(){
        
            std::shared_ptr<KATEModel> rat_model = KATEModel::createModelFromFile(app_Device,"data/models/rat.obj");
            std::shared_ptr<KATEModel> floor_model = KATEModel::createModelFromFile(app_Device,"data/models/quad.obj");
            auto rat = KATEGameObject::createGameObject();
            auto floor = KATEGameObject::createGameObject();
            floor.model = floor_model; //set the model of the game object to be rendered
            floor.transform.translation = {0.f,1.f,4.f};
            floor.transform.scale = {10.f,0.2f,10.f};

            rat.model = rat_model; 
            rat.transform.translation = {0.f,.5f,4.f};
            rat.transform.rotation = {0.f,glm::pi<float>()/2.f,glm::pi<float>()};
            rat.transform.scale = glm::vec3(0.5f);

            auto ratId = rat.getId();
            auto floorId = floor.getId();
            
            gameObjects.emplace(ratId, std::move(rat));
            gameObjects.emplace(floorId, std::move(floor));
            
            std::cout<<"rat id: "<<ratId<<"\n";
            std::cout<<"floor id: "<<floorId<<"\n";

            // Safe access using the actual IDs instead of hardcoded indices
            auto ratIt = gameObjects.find(ratId);
            if (ratIt != gameObjects.end() && ratIt->second.model) {
                std::cout<<"rat loaded with "<<ratIt->second.model->getnumberOfVertices()<<" vertices \n";
            }
            auto floorIt = gameObjects.find(floorId);
            if (floorIt != gameObjects.end() && floorIt->second.model) {
                std::cout<<"floor loaded with "<<floorIt->second.model->getnumberOfVertices()<<" vertices \n";
            }

        }

    void FirstApp::updateFPS(float frameTime) {
        auto now = std::chrono::steady_clock::now();
        currentFPS = (frameTime > 0.0f) ? (1.0f / frameTime) : 0.0f;
        
        // Add current FPS to history
        fpsHistory.push_back({currentFPS, now});
        
        // Add to last 20 frames for ff distribution
        last20Frames.push_back(currentFPS);
        if (last20Frames.size() > MAX_FRAME_SAMPLES) {
            last20Frames.pop_front();
        }
        
        // Remove old entries (older than 20 seconds)
        auto cutoffTime = now - std::chrono::seconds(static_cast<int>(FPS_WINDOW_SECONDS));
        while (!fpsHistory.empty() && fpsHistory.front().timestamp < cutoffTime) {
            fpsHistory.pop_front();
        }
        
        // Calculate statistics
        if (!fpsHistory.empty()) {
            float sum = 0.0f;
            minFPS = std::numeric_limits<float>::max();
            maxFPS = 0.0f;
            
            for (const auto& data : fpsHistory) {
                sum += data.fps;
                minFPS = std::min(minFPS, data.fps);
                maxFPS = std::max(maxFPS, data.fps);
            }
            
            averageFPS = sum / static_cast<float>(fpsHistory.size());
        } else {
            averageFPS = 0.0f;
            minFPS = 0.0f;
            maxFPS = 0.0f;
        }
        
        // Calculate Gaussian distribution for last 20 frames
        calculateGaussianDistribution();
        
        // Calculate advanced statistics
        calculateAdvancedStatistics();
    }

    void FirstApp::calculateGaussianDistribution() {
        if (last20Frames.size() < 2) {
            frameMean = 0.0f;
            frameStdDev = 0.0f;
            gaussianValues.clear();
            return;
        }
        
        // Calculate mean
        float sum = 0.0f;
        for (float fps : last20Frames) {
            sum += fps;
        }
        frameMean = sum / static_cast<float>(last20Frames.size());
        
        // Calculate standard deviation
        float variance = 0.0f;
        for (float fps : last20Frames) {
            variance += (fps - frameMean) * (fps - frameMean);
        }
        variance /= static_cast<float>(last20Frames.size());
        frameStdDev = std::sqrt(variance);
        
        // Generate Gaussian curve values
        gaussianValues.clear();
        if (frameStdDev > 0.001f) { // Avoid division by zero
            float minRange = frameMean - 3.0f * frameStdDev;
            float maxRange = frameMean + 3.0f * frameStdDev;
            const int numPoints = 100;
            
            for (int i = 0; i < numPoints; ++i) {
                float x = minRange + (maxRange - minRange) * static_cast<float>(i) / static_cast<float>(numPoints - 1);
                float y = gaussianFunction(x, frameMean, frameStdDev);
                gaussianValues.push_back(y);
            }
        }
    }

    float FirstApp::gaussianFunction(float x, float mean, float stddev) {
        if (stddev <= 0.001f) return 0.0f;
        
        const float PI = 3.14159265359f;
        float exponent = -0.5f * ((x - mean) / stddev) * ((x - mean) / stddev);
        return (1.0f / (stddev * std::sqrt(2.0f * PI))) * std::exp(exponent);
    }

    void FirstApp::calculateAdvancedStatistics() {
        if (last20Frames.size() < 2) return;
        
        std::vector<float> sortedFrames(last20Frames.begin(), last20Frames.end());
        
        frameMedian = calculateMedian(sortedFrames);
        frameMode = calculateMode(sortedFrames);
        frame25thPercentile = calculatePercentile(sortedFrames, 25.0f);
        frame75thPercentile = calculatePercentile(sortedFrames, 75.0f);
        frame95thPercentile = calculatePercentile(sortedFrames, 95.0f);
        frameSkewness = calculateSkewness(sortedFrames, frameMean, frameStdDev);
        frameKurtosis = calculateKurtosis(sortedFrames, frameMean, frameStdDev);
    }

    float FirstApp::calculateMedian(std::vector<float>& data) {
        if (data.empty()) return 0.0f;
        
        std::sort(data.begin(), data.end());
        size_t size = data.size();
        
        if (size % 2 == 0) {
            return (data[size/2 - 1] + data[size/2]) / 2.0f;
        } else {
            return data[size/2];
        }
    }

    float FirstApp::calculateMode(const std::vector<float>& data) {
        if (data.empty()) return 0.0f;
        
        std::map<int, int> freqMap;
        for (float val : data) {
            int roundedVal = static_cast<int>(std::round(val));
            freqMap[roundedVal]++;
        }
        
        int maxFreq = 0;
        int mode = 0;
        for (const auto& pair : freqMap) {
            if (pair.second > maxFreq) {
                maxFreq = pair.second;
                mode = pair.first;
            }
        }
        
        return static_cast<float>(mode);
    }

    float FirstApp::calculatePercentile(std::vector<float>& data, float percentile) {
        if (data.empty()) return 0.0f;
        
        std::sort(data.begin(), data.end());
        float index = (percentile / 100.0f) * (data.size() - 1);
        int lowerIndex = static_cast<int>(std::floor(index));
        int upperIndex = static_cast<int>(std::ceil(index));
        
        if (lowerIndex == upperIndex) {
            return data[lowerIndex];
        }
        
        float weight = index - lowerIndex;
        return data[lowerIndex] * (1.0f - weight) + data[upperIndex] * weight;
    }

    float FirstApp::calculateSkewness(const std::vector<float>& data, float mean, float stddev) {
        if (data.size() < 3 || stddev <= 0.001f) return 0.0f;
        
        float sum = 0.0f;
        for (float val : data) {
            float deviation = (val - mean) / stddev;
            sum += deviation * deviation * deviation;
        }
        
        return sum / data.size();
    }

    float FirstApp::calculateKurtosis(const std::vector<float>& data, float mean, float stddev) {
        if (data.size() < 4 || stddev <= 0.001f) return 0.0f;
        
        float sum = 0.0f;
        for (float val : data) {
            float deviation = (val - mean) / stddev;
            sum += deviation * deviation * deviation * deviation;
        }
        
        return (sum / data.size()) - 3.0f; // Excess kurtosis
    }

    void FirstApp::renderFPSWindow() {
        ImGui::Begin("Last 20 Seconds Average FPS, Max FPS and Min FPS");
        
        ImGui::Text("FPS Statistics (Last 20 seconds)");
        ImGui::Separator();
        
        ImGui::Text("Current FPS: %.1f", currentFPS);
        ImGui::Text("Average FPS: %.1f", averageFPS);
        ImGui::Text("Maximum FPS: %.1f", maxFPS);
        ImGui::Text("Minimum FPS: %.1f", minFPS);
        
        ImGui::Separator();
        ImGui::Text("Sample Count: %zu", fpsHistory.size());
        
        // FPS history graph
        if (!fpsHistory.empty()) {
            std::vector<float> fpsValues;
            fpsValues.reserve(fpsHistory.size());
            for (const auto& data : fpsHistory) {
                fpsValues.push_back(data.fps);
            }
            
            ImGui::PlotLines("FPS History", fpsValues.data(), static_cast<int>(fpsValues.size()), 
                           0, nullptr, 0.0f, maxFPS * 1.1f, ImVec2(0, 80));
        }
        
        ImGui::Separator();
        
        // Last 20 frames statistics and Gaussian distribution
        ImGui::Text("Last 20 Frames Analysis");
        ImGui::Text("Sample Count: %zu", last20Frames.size());
        if (last20Frames.size() >= 2) {
            ImGui::Text("Mean FPS: %.2f", frameMean);
            ImGui::Text("Standard Deviation: %.2f", frameStdDev);
            
            // Advanced statistical analysis
            ImGui::Separator();
            ImGui::Text("Statistical Analysis (Last 20 frames):");
            
            ImGui::Columns(2, "StatsColumns", false);
            
            ImGui::Text("Median: %.2f fps", frameMedian);
            ImGui::Text("Mode: %.2f fps", frameMode);
            ImGui::Text("25th Percentile: %.2f fps", frame25thPercentile);
            
            ImGui::NextColumn();
            
            ImGui::Text("75th Percentile: %.2f fps", frame75thPercentile);
            ImGui::Text("95th Percentile: %.2f fps", frame95thPercentile);
            ImGui::Text("Skewness: %.3f", frameSkewness);
            ImGui::Text("Kurtosis: %.3f", frameKurtosis);
            
            ImGui::Columns(1);
            
            // Interpretation of skewness and kurtosis
            ImGui::Separator();
            ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "Interpretation:");
            if (frameSkewness > 0.1f) {
                ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "→ Positive skew: Some high frame spikes");
            } else if (frameSkewness < -0.1f) {
                ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "→ Negative skew: Some low frame drops");
            } else {
                ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "→ Symmetric distribution");
            }
            
            if (frameKurtosis > 0.5f) {
                ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "→ High kurtosis: Peaked, unstable FPS");
            } else if (frameKurtosis < -0.5f) {
                ImGui::TextColored(ImVec4(0.0f, 0.8f, 1.0f, 1.0f), "→ Low kurtosis: Flat, stable FPS");
            } else {
                ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "→ Normal distribution shape");
            }
            
            // Show actual frame values (collapsible)
            if (ImGui::CollapsingHeader("Frame Values")) {
                ImGui::Text("Individual frame FPS values:");
                for (size_t i = 0; i < last20Frames.size(); ++i) {
                    if (i > 0 && i % 5 == 0) ImGui::NewLine();
                    ImGui::SameLine();
                    ImGui::Text("%.1f", last20Frames[i]);
                }
                ImGui::NewLine();
            }
            
        } else {
            ImGui::Text("Need at least 2 frames for analysis");
        }
        
        ImGui::End();
    }

}