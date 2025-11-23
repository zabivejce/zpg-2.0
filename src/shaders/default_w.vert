#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
  
out vec3 color;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform float w = 10.0; // pouziti na kouli (je mensi)

void main()
{
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    gl_Position = projection * view * model * vec4(aPos, w);
    color = aColor;
}