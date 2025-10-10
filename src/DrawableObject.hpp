#pragma once
#include "Model.hpp"
#include "ShaderProgram.hpp"
class DrawableObject
{
    private:
        Model* model;
        ShaderProgram* shader;
        glm::mat4 M;
    public:
        DrawableObject(Model* m, ShaderProgram* sp);
        void draw();
};