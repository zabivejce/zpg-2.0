#pragma once
#include "DrawableObject.hpp"
#include "Camera.hpp"
#include "ShaderProgram.hpp"
#include "Light.hpp"

#include <vector>
class Scene
{
    private:
        std::vector<DrawableObject*> objects;
        std::vector<Light*> lights;
        Camera* camera;
    public:
        Scene(std::vector<ShaderProgram*>& shaders, std::vector<Light*>& lights);
        void addObject(DrawableObject* obj);
        void drawScene();
        Camera* getCamnera();
};