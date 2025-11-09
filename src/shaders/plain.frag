#version 330 core

out vec4 FragColor;

uniform sampler2D texUnit;
uniform int lightCount;
uniform int haveTex;

in vec2 TexCoord;

void main()
{
    if(haveTex == 1)
        FragColor = texture(texUnit, TexCoord);
    else
        FragColor = vec4(1.0,0.0,0.0,1.0);
}