#pragma once
#include "kate_model.hpp"
#include <memory>

namespace kate{
    class KATEGameObject{
        struct Transform2dComponent{
            glm::vec2 translation{}; //position offset, this will be used to move things up and down
            glm::vec2 scale{1.f,1.f};
            float rotation;
            glm::mat2 mat2(){
                const float s = glm::sin(rotation);
                const float c = glm::cos(rotation);
                glm::mat2 rotational_Matrix({c,s},{-s,c});
                glm::mat2 scale_Matrix{{scale.x,.0f},{.0f,scale.y}}; //GLM USES COLUMNS, NOT ROWS

                return rotational_Matrix*scale_Matrix;
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
            Transform2dComponent transform2d{};
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