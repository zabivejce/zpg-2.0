#include "Scene.hpp"
#include "DrawableObject.hpp"
Scene::Scene(std::vector<ShaderProgram*>& shaders,std::vector<Light*>& lights)
{
    camera = Camera::getInstance(glm::vec3(0.0f,0.0f,1.0f),glm::vec3(0.0f,0.0f,-1.0f), glm::vec3(0.0f,1.0f,0.0f));
    for(auto* sh : shaders)
        camera->registerShader(sh);
    this->lights = lights;
}
Scene::Scene(std::vector<ShaderProgram*>& shaders)
{
    camera = Camera::getInstance(glm::vec3(0.0f,0.0f,1.0f),glm::vec3(0.0f,0.0f,-1.0f), glm::vec3(0.0f,1.0f,0.0f));
    for(auto* sh : shaders)
        camera->registerShader(sh);
}

void Scene::addObject(DrawableObject* obj)
{
    objects.emplace_back(obj);
    if(lights.size() > 0)
        obj->setLights(lights);
}

void Scene::drawScene()
{
    if(skybox != nullptr)
        skybox->draw(camera->getProjection(), camera->getView());

    camera->matrix(90.0f,0.1f,100.0f);

    for(auto* obj : objects)
        obj->draw(camera);
}

Camera* Scene::getCamnera()
{
    return camera;
}

void Scene::update(float delta)
{
    for(auto* i : objects)
        i->update(delta);
}

void Scene::removeObj(int id)
{
    for(int i = 0 ; i < objects.size() ; ++i)
    {
        if(objects[i]->getId() == id)
        {
            delete objects[i];
            objects.erase(objects.begin() + i); // removne N-ty objekt z vektoru
            return;
        }
    }
}