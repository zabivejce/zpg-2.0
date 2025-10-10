#include "DrawableObject.hpp"
DrawableObject::DrawableObject(Model* m, ShaderProgram* sp, TransformationComponent* tr)
{
    model = m;
    shader = sp;
    transformation = tr;
}
void DrawableObject::draw()
{
    glm::mat4 M = transformation->getModelMatrix();
    shader->setProgram();
    shader->setUniform("model", M);
    model->drawModel();
    shader->resetProgram();
}