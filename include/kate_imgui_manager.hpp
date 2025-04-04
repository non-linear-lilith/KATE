#pragma once

#include "kate_device.hpp"
#include "kate_swap_chain.hpp"
#include "kate_window.hpp"

#include <imgui.h>

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>

namespace kate {

class KATEImGuiManager {
public:
/**
 * @brief Construct a new KATEImGuiManager object  
 * 
 * @param window current window used by vulkan
 * @param device the device is the physical device used by vulkan
 * @param renderPass this is the render pass used by vulkan
 * @param imageCount the number of images in the swap chain
 */
    KATEImGuiManager(
        KATEWindow& window,
        KATEDevice& device,
        VkRenderPass renderPass,
        uint32_t imageCount);
    ~KATEImGuiManager();

    // Prevent copying
    KATEImGuiManager(const KATEImGuiManager&) = delete;
    KATEImGuiManager& operator=(const KATEImGuiManager&) = delete;

    // Start a new frame
    void newFrame();
    
    // Render the ImGui frame
    void render(VkCommandBuffer commandBuffer);

private:
    void init();
    void createDescriptorPool();

    KATEWindow& kateWindow;
    KATEDevice& kateDevice;
    VkRenderPass renderPass;
    uint32_t imageCount;
    
    VkDescriptorPool descriptorPool = VK_NULL_HANDLE;
};

} // namespace kate