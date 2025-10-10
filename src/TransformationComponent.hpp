#pragma once

#include <glm/glm.hpp>

class TransformationComponent
{
    private:
    public:
        virtual glm::mat4 getModelMatrix() = 0;
        virtual void add(TransformationComponent* component) {}
};