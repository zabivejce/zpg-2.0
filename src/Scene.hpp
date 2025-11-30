#pragma once
#include "DrawableObject.hpp"
#include "Camera.hpp"
#include "ShaderProgram.hpp"
#include "SkyBox.hpp"

#include <vector>
class Scene
{
    private:
        std::vector<DrawableObject*> objects;
        std::vector<Light*> lights;
        Camera* camera;
        SkyBox* skybox = nullptr;
    public:
        Scene(std::vector<ShaderProgram*>& shaders, std::vector<Light*>& lights);
        Scene(std::vector<ShaderProgram*>& shaders);
        void addObject(DrawableObject* obj);
        void setSkyBox(std::vector<std::string> faces, ShaderProgram* shader)
        {skybox = new SkyBox(faces,shader);}
        void drawScene();
        void update(float delta);
        void removeObj(int id);
        Camera* getCamnera();
};