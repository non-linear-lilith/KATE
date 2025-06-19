#pragma once
#include "kate_model.hpp"
#include <memory>
#include <unordered_map>
#include <glm/gtc/matrix_transform.hpp>
namespace kate{
    struct TransformComponent{
        
        glm::vec3 translation{}; // 3D traslation of the object with a x,y,z coordinate
        glm::vec3 scale{1.f,1.f,1.f}; // 3D scale of the object with a x,y,z coordinate
        glm::vec3 rotation{}; // 3D rotation of the object with a x,y,z coordinate. //@note I should insted use 2D for rotation using a normaliced vector and 2 angles or a quaternion
        /**
         * @param none
         * @brief Fenerates a 4x4 matrix from the translation, scale and rotation values of the object to be used in the vertex shader
         *  it uses the rotation values to generate a rotation matrix (eulers angles) and then multiplies it with the scale and translation matrices
         * @return A 4x4 matrix in the form of a glm::mat4
         * @note I should use quaternions instead of euler angles for rotation
         */
        glm::mat4 mat4();
        /**
         * @param none
         * @brief Fenerates a 3x3 matrix from the rotation values of the object to be used in the vertex shader
         *  it uses the rotation values to generate a rotation matrix (eulers angles)
         * @return A 3x3 matrix in the form of a glm::mat3
         * @note I should use quaternions instead of euler angles for rotation
         */
        glm::mat3 normalMatrix();
    };

    class KATEGameObject{
        
        public:
            using id_t = unsigned int;
            using Map = std::unordered_map<id_t, KATEGameObject>;

            static KATEGameObject createGameObject() {
                static id_t currentId = 0;
                return KATEGameObject{currentId++};
            }

            KATEGameObject(const KATEGameObject &) = delete;
            KATEGameObject &operator=(const KATEGameObject &) = delete;
            KATEGameObject(KATEGameObject &&) = default;
            KATEGameObject &operator=(KATEGameObject &&) = default;

            id_t getId() { return id; }

            std::shared_ptr<KATEModel> model{};
            glm::vec3 color{};
            TransformComponent transform{};

        private:
            KATEGameObject(id_t objId):id(objId){}
            id_t id;
    };
}