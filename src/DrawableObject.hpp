#pragma once
#include "Model.hpp"
#include "ShaderProgram.hpp"
#include "TransformationComponent.hpp"
#include "Camera.hpp"
#include <vector>
class DrawableObject
{
    private:
        int id;
        Model* model;
        ShaderProgram* shader;
        TransformationComponent* transformation;
        std::vector<Light*> lights;
    public:
        DrawableObject(Model* m, ShaderProgram* sp, TransformationComponent* tr, int id = 0);
        int getId(){return id;};
        void draw(Camera* camera);
        void setLights(std::vector<Light*> lights)
        {this->lights = lights;}
        void update(float delta)
        {transformation->update(delta);}
};