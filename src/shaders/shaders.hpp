#pragma once
inline const char* vert_sh = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
  
out vec3 color;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    color = normalize(aColor) * 0.5 + 0.5;
}
)";
inline const char* frag_sh = R"(
#version 330 core
in vec3 color;
out vec4 FragColor;

void main()
{
    FragColor = vec4(color, 1.0);
}
)";

//depricated :)
inline float points_trg[] = {
0.0f, 0.5f, 0.0f,
0.5f, -0.5f, 0.0f,
-0.5f, -0.5f, 0.0f
};
inline float points_sqr[] = {
0.6f, 0.6f, 0.0f,
0.8f, 0.6f, 0.0f,
0.8f, 0.8f, 0.0f,
0.6f, 0.6f, 0.0f,
0.6f, 0.8f, 0.0f,
0.8f, 0.8f, 0.0f
};