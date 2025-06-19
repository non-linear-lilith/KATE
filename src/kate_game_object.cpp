#include <kate_game_object.hpp>
namespace kate{

            glm::mat4 TransformComponent::mat4() {
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
            glm::mat3 TransformComponent::normalMatrix(){
                const float c3 = glm::cos(rotation.z);
                const float s3 = glm::sin(rotation.z);
                const float c2 = glm::cos(rotation.x);
                const float s2 = glm::sin(rotation.x);
                const float c1 = glm::cos(rotation.y);
                const float s1 = glm::sin(rotation.y);
                const glm::vec3 invScale{1.f/scale.x, 1.f/scale.y, 1.f/scale.z};

                return glm::mat3{
                    {
                        invScale.x * (c1 * c3 + s1 * s2 * s3),
                        invScale.x * (c2 * s3),
                        invScale.x * (c1 * s2 * s3 - c3 * s1)
                    },
                    {
                        invScale.y * (c3 * s1 * s2 - c1 * s3),
                        invScale.y * (c2 * c3),
                        invScale.y * (c1 * c3 * s2 + s1 * s3)
                    },
                    {
                        invScale.z * (c2 * s1),
                        invScale.z * (-s2),
                        invScale.z * (c1 * c2)
                    }
                }; 
            }
}


    /**
             * WARNING, THIS FUNCTION IS LESS EFFICIENT THAN CREATING YOUR OWN FUNCTION WITH EULER SYSTEM
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