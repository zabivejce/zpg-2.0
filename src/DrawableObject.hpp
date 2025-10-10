#pragma once
#include "Model.hpp"
#include "ShaderProgram.hpp"
#include "TransformationComponent.hpp"
class DrawableObject
{
    private:
        Model* model;
        ShaderProgram* shader;
        TransformationComponent* transformation;
    public:
        DrawableObject(Model* m, ShaderProgram* sp, TransformationComponent* tr);
        void draw();
};