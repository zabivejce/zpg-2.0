#include "DrawableObject.hpp"
#include "Camera.hpp"
DrawableObject::DrawableObject(Model* m, ShaderProgram* sp, TransformationComponent* tr)
{
    model = m;
    shader = sp;
    transformation = tr;
}
void DrawableObject::draw(Camera* camera)
{
    glm::mat4 M = transformation->getModelMatrix();
    shader->setProgram();
    shader->setUniform("model", M);
    shader->setUniform("view",glm::mat4(glm::mat3(camera->getView())));
    shader->setUniform("projection",camera->getProjection());
    model->drawModel();
    shader->resetProgram();
}