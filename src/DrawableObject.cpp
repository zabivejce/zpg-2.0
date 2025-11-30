#include "DrawableObject.hpp"
#include "Camera.hpp"
DrawableObject::DrawableObject(Model* m, ShaderProgram* sp, TransformationComponent* tr, Material* mat, int id)
{
    model = m;
    shader = sp;
    transformation = tr;
    material = mat;
    this->id =id;
}
void DrawableObject::draw(Camera* camera)
{
    if(id != 0)
        glStencilFunc(GL_ALWAYS, id, 0xFF);
    else
        glStencilFunc(GL_ALWAYS, 0, 0xFF); 

    glm::mat4 M = transformation->getModelMatrix();
    shader->setProgram();
    shader->setUniform("model", M);
    if(material != nullptr)
        shader->setMaterial(material);
    shader->setUniform("view",camera->getView());
    shader->setUniform("projection",camera->getProjection());
    if(lights.size() > 0)
        shader->setLights(lights);
    if(model->doHaveTexture())
        shader->setUniform("texUnit",0);
    shader->setUniform("haveTex",model->doHaveTexture());
    model->drawModel();
    shader->resetProgram();
}