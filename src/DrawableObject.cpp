#include "DrawableObject.hpp"
DrawableObject::DrawableObject(Model* m, ShaderProgram* sp)
{
    model = m;
    shader = sp;
}
void DrawableObject::draw()
{
    shader->setProgram();
    model->drawModel();
}