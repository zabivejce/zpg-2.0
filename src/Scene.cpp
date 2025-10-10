#include "Scene.hpp"
Scene::Scene(std::vector<ShaderProgram*>& shaders)
{
    camera = Camera::getInstance(glm::vec3(0.0f,0.0f,1.0f),glm::vec3(0.0f,0.0f,-1.0f), glm::vec3(0.0f,1.0f,0.0f));
    for(auto* sh : shaders)
        camera->registerShader(sh);
}

void Scene::addObject(DrawableObject* obj)
{
    objects.emplace_back(obj);
}

void Scene::drawScene()
{
    camera->matrix(170.0f,0.1f,100.0f);
    for(auto* obj : objects)
        obj->draw(camera);
}

Camera* Scene::getCamnera()
{
    return camera;
}