#pragma once
#include "Model.hpp"
#include "ShaderProgram.hpp"
#include "TransformationComponent.hpp"
#include "Camera.hpp"
#include <vector>
class DrawableObject
{
    private:
        Model* model;
        ShaderProgram* shader;
        TransformationComponent* transformation;
        std::vector<Light*> lights;
    public:
        DrawableObject(Model* m, ShaderProgram* sp, TransformationComponent* tr);
        void draw(Camera* camera);
        void setLights(std::vector<Light*> lights)
        {this->lights = lights;}
};