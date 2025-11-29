#pragma once
#include "Light.hpp"
#include <glm/ext/vector_float3.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>
class SpotLight : public Light
{
    private:
    protected:
        float angle;
        bool active = true;
        glm::vec3 position;
        glm::vec3 direction;
        glm::vec3 attenuation;
    public:
        SpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 attenuation, float angle)
        {
            this->angle = angle;
            this->position = position;
            this->direction = direction;
            this->attenuation = attenuation;
        }
        int getType() override
        {return 3;}
        float getAngle()
        {return glm::cos(glm::radians(angle));}
        glm::vec3 getPosition()
        {return position;}
        glm::vec3 getDirection()
        {return direction;}
        glm::vec3 getAttenuation()
        {return attenuation;}
        bool isActive()
        {return active;}
};