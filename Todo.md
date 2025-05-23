# KATE Project Todo List

## 📋 Overview
This document tracks tasks, bugs, and feature requests for the KATE project.

- [X] Fix vulkan buggs (4) 
    - [X] Vulkan error 1/4 
        - Validation Layer: Validation Error: [ VUID-vkFreeDescriptorSets-descriptorPool-parameter ] Object 0: handle = 0x1df9dbef6d0, type = VK_OBJECT_TYPE_INSTANCE; | MessageID = 0xfc0204f1 | vkFreeDescriptorSets(): descriptorPool Invalid VkDescriptorPool Object 0xcfcda0000000001e. The Vulkan spec states: descriptorPool must be a valid VkDescriptorPool handle (https://vulkan.lunarg.com/doc/view/1.4.304.0/windows/1.4-extensions/vkspec.html#VUID-vkFreeDescriptorSets-descriptorPool-parameter)
    - [X] Vulkan error 2/4
        -  Validation Layer: Validation Error: [ VUID-vkFreeDescriptorSets-pDescriptorSets-00310 ] Object 0: handle = 0xb9181f0000000029, type = VK_OBJECT_TYPE_DESCRIPTOR_SEriptorSet 0xb9181f0000000029[]. The Vulkan spec states: pDescriptorSets must be a valid pointer to an array of descriptorSetCount VkDescriptorSet handles, each element of which must either be a valid handle or VK_NULL_HANDLE (https://vulkan.lunarg.com/doc/view/1.4.304.0/windows/1.4riptorSet 0xb9181f0000000029[]. The Vulkan spec states: pDescriptorSets must be a valid pointer to an array of descriptorSetCount VkDescriptorSet handles, each element of which must either be a valid handle or VK_NULL_HANDLE (https://vulkan.lunarg.com/doc/view/1.4.304.0/windows/1.4-extensions/vkspec.html#VUID-vkFreeDescriptorSets-pDescriptorSets-00310)
    - [X] Vulkan error 3/4
        -  Validation Layer: Validation Error: [ UNASSIGNED-Threading-Info ] Object 0: handle = 0xcfcda0000000001e, type = VK_OBJECT_TYPE_DESCRIPTOR_POOL; | MessageID = 0x5d6b67e2 | vkFreeDescriptorSets(): Couldn't find VkDescriptorPool Object 0xcfcda0000000001e. This should not happen and may indicate a bug in the application.
    - [X] Vulkan error 4/4
        -  Validation Layer: Validation Error: [ UNASSIGNED-Threading-Info ] Object 0: handle = 0xcfcda0000000001e, type = VK_OBJECT_TYPE_DESCRIPTOR_POOL; | MessageID = 0x5d6b67e2 | vkFreeDescriptorSets(): Couldn't find VkDescriptorPool Object 0xcfcda0000000001e. This should not happen and may indicate a bug in the application.
- [ ] Delete imgui unused files in "external/imgui"
- [ ] in "include\kate_game_object.hpp" I should change the vec2 to vec3 parameters. or use z coordinate for something like radius.
- [OK] Optimize rotation function
- [ ] Add quaternions to rotation position for mouse and camera rotation.
- [ ] Fix library implementation in CmakeLists.txt