#pragma once
#include "kate_model.hpp"
#include <memory>

#include <glm/gtc/matrix_transform.hpp>
namespace kate{
    class KATEGameObject{
        struct TransformComponent{
            glm::vec3 translation{}; //position offset, this will be used to move things up and down
            glm::vec3 scale{1.f,1.f,1.f};
            glm::vec3 rotation;
            /**
             * WARNING, THIS SYSTEM IS LESS EFFICIENT THAN CREATING YOUR OWN FUNCTION WITH EULER SYSTEM
            //Euler Angles rotation system

            //MAtrix corresponds to translate*Ry*Rx*Rz*scale transformation
            //Rotation convention uses Tait-Bryan angles with axis order Y(1),X(2),Z(3)
            glm::mat4 mat4(){
                auto transform = glm::translate(glm::mat4{1.f},translation);

                transform = glm::rotate(transform,rotation.y,{0.f,1.f,0.f});
                transform = glm::rotate(transform,rotation.x,{1.f,0.f,0.f});
                transform = glm::rotate(transform,rotation.z,{0.f,0.f,1.f});

                transform = glm::scale(transform,scale);
                
                return transform;
            } **/
            glm::mat4 mat4() {
                const float c3 = glm::cos(rotation.z);
                const float s3 = glm::sin(rotation.z);
                const float c2 = glm::cos(rotation.x);
                const float s2 = glm::sin(rotation.x);
                const float c1 = glm::cos(rotation.y);
                const float s1 = glm::sin(rotation.y);
                return glm::mat4{
                    {
                        scale.x * (c1 * c3 + s1 * s2 * s3),
                        scale.x * (c2 * s3),
                        scale.x * (c1 * s2 * s3 - c3 * s1),
                        0.0f,
                    },
                    {
                        scale.y * (c3 * s1 * s2 - c1 * s3),
                        scale.y * (c2 * c3),
                        scale.y * (c1 * c3 * s2 + s1 * s3),
                        0.0f,
                    },
                    {
                        scale.z * (c2 * s1),
                        scale.z * (-s2),
                        scale.z * (c1 * c2),
                        0.0f,
                    },
                    {translation.x, translation.y, translation.z, 1.0f}
                };
            }
        };

        public:
            using id_t = unsigned int;

            static KATEGameObject createGameObject(){
                static id_t currentId = 0;
                return KATEGameObject{currentId++};
            }
            id_t getId(){
                return id;
            }
            TransformComponent transform{};
            std::shared_ptr<KATEModel>model{};
            glm::vec3 color{};

            KATEGameObject(const KATEGameObject&) = delete;
            KATEGameObject &operator=(const KATEGameObject&&) = delete;
            KATEGameObject(KATEGameObject&&) = default;
            KATEGameObject &operator=(KATEGameObject&&) = default;

        private:
            KATEGameObject(id_t objId):id(objId){
            }
            id_t id;
    };
}