#pragma once
#include "TransformationComponent.hpp"
class CustomTransformation : public TransformationComponent
{
    private:
    public:
    glm::mat4 getModelMatrix() override
    {
        glm::mat4 M(1.0f); // taky se zmensi visco
        M[3][3] = 20.0f;
        return M;
    }
};