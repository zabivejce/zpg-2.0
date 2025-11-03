#pragma once
#include "Light.hpp"
#include <glm/vec3.hpp>
class DirectionLight : public Light
{
    private:
        glm::vec3 direction;
    public:
        DirectionLight(glm::vec3 direction)
        {this->direction = direction;}
        int getType() override
        {return 2;}
        glm::vec3 getDirection()
        {return direction;}
};