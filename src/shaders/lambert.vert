#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
//uniform mat3 normalMatrix; //(M-1)T

out vec4 ex_worldPosition;
out vec3 ex_worldNormal;


void main(void){
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    gl_Position = projection * view * model * vec4(aPos,1.0);
    ex_worldPosition = model * vec4(aPos,1.0f);
    ex_worldNormal = normalMatrix * aNormal;
}