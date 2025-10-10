#pragma once
#include "DrawableObject.hpp"
#include "Camera.hpp"
#include "ShaderProgram.hpp"

#include <vector>
class Scene
{
    private:
        std::vector<DrawableObject*> objects;
        Camera* camera;
    public:
        Scene(std::vector<ShaderProgram*>& shaders);
        void addObject(DrawableObject* obj);
        void drawScene();
        Camera* getCamnera();
};