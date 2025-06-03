#pragma once

#include "kate_game_object.hpp"
#include "kate_window.hpp"

namespace kate{
    class KeyboardInput{
        public:
            struct KeyMappings {
                uint16_t moveLeft = GLFW_KEY_A;
                uint16_t moveRight = GLFW_KEY_D;
                uint16_t moveForward = GLFW_KEY_W;
                uint16_t moveBackward = GLFW_KEY_S;
                uint16_t moveUp = GLFW_KEY_E;
                uint16_t moveDown = GLFW_KEY_Q;
                
                uint16_t lookLeft = GLFW_KEY_LEFT;
                uint16_t lookRight = GLFW_KEY_RIGHT;
                uint16_t lookUp = GLFW_KEY_UP;
                uint16_t lookDown = GLFW_KEY_DOWN;

                uint16_t move_forward = GLFW_KEY_I;
                uint16_t move_backward = GLFW_KEY_K;
                uint16_t move_left = GLFW_KEY_J;
                uint16_t move_right = GLFW_KEY_L;
            };
            void moveInPlaneXZ(GLFWwindow* window, float dt, KATEGameObject& gameObject);
            void movepos(GLFWwindow* window, float dt, KATEGameObject& gameObject);
            KeyMappings keys{};
            float movespeed{3.f};
            float lookSpeed{1.5f};
    };
}