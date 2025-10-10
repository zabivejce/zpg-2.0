#pragma once

#include "TransformationComponent.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class Translation : public TransformationComponent
{
    private:
        glm::vec3 position;
    public:
        Translation(glm::vec3 position)
        {
            this->position = position;
        }
        glm::mat4 getModelMatrix() override
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, position);
            return model;
        }
};