#version 330 core
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(void)
{
    gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * vec4(vp,1.0f);
    worldPosition = modelMatrix * vec4(vp,1.0f);
    worldNormal = vn; //next lesson
}