#pragma once
#include "TransformationComponent.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <random>
class MoleDynamicTranslation : public TransformationComponent
{
    private:
        glm::vec3 begin;
        glm::vec3 end;
        float speed;
        float t;
        float pause;
        float getRandomRange(float begin, float end)
        {
            static std::random_device rd;
            static std::mt19937 gen(rd());
            std::uniform_real_distribution<float> dist(begin, end);
            return dist(gen);
        }
    public:
        MoleDynamicTranslation(glm::vec3 begin, glm::vec3 end)
        {
            this->begin = begin;
            this->end = end;
            speed = getRandomRange(1.0, 3.0);
            t = 0.0f;
        }
        void update(float delta) override
        {
            if(pause <= 0)
            {
                t += speed * delta;
                if(t >= 1.0f)
                {
                    t = 1.0f;
                    speed = -speed;
                }
                else if(t <= 0.0f)
                {
                    t = 0.0f;
                    pause = getRandomRange(1.0f, 5.0f);
                    speed = getRandomRange(1.0, 3.0); //musi byt pozitivni, protoze jdou nahoru
                }
                pause -= delta;
            }
            else
            {
                pause -= delta;
            }
        }
        glm::mat4 getModelMatrix() override
        {
            // parametricka rovnice primky: P = A + t * (B - A)
            glm::vec3 curr = begin + t * (end - begin);
            return glm::translate(glm::mat4(1.0f), curr);
        }
};