#pragma once
#include "TransformationComponent.hpp"
#include <glm/ext/matrix_float4x3.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/matrix.hpp>
#include <vector>
class BezierTranslation : public TransformationComponent
{
    private:
        glm::mat4 A;    //bersteinovo matice
        std::vector<glm::mat4x3> segments;
        float speed;
        float t;
        int activeSeg;
        bool rotate;
    public:
        BezierTranslation(std::vector<glm::mat4x3> segments, float speed, bool rotate=true)
        {
            this->segments = segments;
            this->speed = speed;
            t = 0.0f;
            activeSeg = 0;
            this->rotate = rotate;
            A = glm::mat4(
                glm::vec4(-1.0, 3.0, -3.0, 1.0),
                glm::vec4(3.0, -6.0, 3.0, 0),
                glm::vec4(-3.0, 3.0, 0, 0),
                glm::vec4(1, 0, 0, 0)
            );
        }
        void update(float delta) override
        {
            t += speed * delta;
            if(t > 1.0f)
            {
                t -= 1.0f;  //vrati se na zacatek
                ++activeSeg;
                if(activeSeg >= segments.size())
                    activeSeg = 0;
            }
        }
        glm::mat4 getModelMatrix() override
        {
            float t2 = t*t;
            float t3 = t*t*t;
            glm::vec4 params = glm::vec4(t3,t2,t,1);
            glm::vec3 trans = params * A * glm::transpose(segments[activeSeg]);
            glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), trans);
            if(rotate)
            {
                //derivcace - tecny vektor
                glm::vec4 parametersDt = glm::vec4(3 * t2, 2 * t, 1.0f, 0.0f);

                glm::vec3 tangent = parametersDt * A * glm::transpose(segments[activeSeg]);
                glm::vec3 forward = glm::normalize(tangent);

                glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

                glm::vec3 right = glm::normalize(glm::cross(forward, worldUp));

                glm::vec3 up = glm::cross(right, forward);

                glm::mat4 rotation = glm::mat4(1.0f);
                rotation[0] = glm::vec4(right, 0.0f);
                rotation[1] = glm::vec4(up, 0.0f);
                rotation[2] = glm::vec4(-forward, 0.0f);

                return translationMatrix * rotation;
            }
            return translationMatrix;
        }
};