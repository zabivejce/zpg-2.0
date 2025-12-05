#include "PointLight.hpp"
#include "BezierTranslation.hpp"
#include "TransformationComponent.hpp"
#include <cstddef>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>
#include <random>
class WhispDynamicTranslation : public TransformationComponent
{
    private:
        //BezierTranslation* bezier;
        PointLight* light;
        glm::vec3 startPos;
        glm::vec3 range;
        glm::vec3 currPos;
        glm::vec3 target;
        float speed;
        float getRandomRange(float begin, float end)
        {
            static std::random_device rd;
            static std::mt19937 gen(rd());
            std::uniform_real_distribution<float> dist(begin, end);
            return dist(gen);
        }
        void newTarget()
        {
            target.x = getRandomRange(startPos.x - range.x, startPos.x + range.x);
            target.y = getRandomRange(startPos.y - range.y, startPos.y + range.y);
            target.z = getRandomRange(startPos.z - range.z, startPos.z + range.z);
        }
    public:
        WhispDynamicTranslation(PointLight* light, glm::vec3 start, glm::vec3 range, float speed)
        {
            this->light = light;
            startPos = start;
            currPos = start;
            this->range = range;
            this->speed = speed;
        }
        void update(float delta) override
        {
            glm::vec3 dir = target - currPos;
            float dist = glm::length(dir);
            if(dist <= 0.1f)
                newTarget();
            else
                currPos += glm::normalize(dir) * speed * delta;

            if(light != nullptr)
                light->setPosition(currPos);
        }
        glm::mat4 getModelMatrix() override
        {return glm::translate(glm::mat4(1.0f),currPos);}
};