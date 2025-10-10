#include "DrawableObject.hpp"
DrawableObject::DrawableObject(Model* m, ShaderProgram* sp)
{
    model = m;
    shader = sp;
    M = glm::mat4(1.0f);

    //M = glm::rotate(glm::mat4(1.0f),1.0f,glm::vec3(0.0f, 1.0f, 0.0f));
    //M = glm::rotate(M, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    //M = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    //M = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
}
void DrawableObject::draw()
{
    shader->setUniform("model", M);
    shader->setProgram();
    model->drawModel();
}