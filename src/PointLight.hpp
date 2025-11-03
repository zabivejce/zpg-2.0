#pragma once
#include "Light.hpp"
#include <glm/vec3.hpp>
class PointLight : public Light
{
    private:
        glm::vec3 position;
        glm::vec3 attenuation;
    public:
        PointLight(glm::vec3 position, glm::vec3 attenuation)
        {
            this->position = position;
            this->attenuation = attenuation;
        }
        int getType() override
        {return 1;}
        glm::vec3 getPosition()
        {return position;}
        glm::vec3 getAttenuation()
        {return attenuation;}

};