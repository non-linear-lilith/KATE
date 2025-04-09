// kate_text_renderer.hpp
#pragma once

#include "kate_device.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glm/glm.hpp>
#include <map>

namespace kate {

struct Character {
    VkImage textureImage;
    VkDeviceMemory textureImageMemory;
    VkImageView textureView;
    glm::ivec2 size;
    glm::ivec2 bearing;
    unsigned int advance;
};

class KATETextRenderer {
public:
    KATETextRenderer(KATEDevice& device);
    ~KATETextRenderer();

    void renderText(VkCommandBuffer commandBuffer, const std::string& text, float x, float y, float scale, glm::vec3 color);
    void cleanup();

private:
    void init();
    void createTextPipeline();
    void loadCharacters();

    KATEDevice& device;
    FT_Library ft;
    FT_Face face;
    std::map<char, Character> characters;
    VkPipelineLayout pipelineLayout;
    VkPipeline textPipeline;
    VkDescriptorSetLayout descriptorSetLayout;
    VkDescriptorPool descriptorPool;
};

}