#pragma once
#include "TransformationComponent.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>

class Rotation : public TransformationComponent
{
    private:
        glm::vec3 rotation;
    public:
        Rotation(glm::vec3 rotation)
        {
            this->rotation = rotation;
        }
        glm::mat4 getModelMatrix() override
        {
            glm::mat4 model = glm::mat4(1.0f);

            model = glm::rotate(model, glm::radians(rotation.x),glm::vec3(1.0f,0.0f,0.0f));
            model = glm::rotate(model, glm::radians(rotation.y),glm::vec3(0.0f,1.0f,0.0f));
            model = glm::rotate(model, glm::radians(rotation.z),glm::vec3(0.0f,0.0f,1.0f));

            return model;
        }
};