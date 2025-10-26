#version 330 core
out vec4 FragColor;
in vec4 ex_worldPosition;

in vec3 ex_worldNormal;
out vec4 out_Color;

uniform vec3 eye = vec3(0.0,0.0,3.0);
uniform vec3 lightPosition;

void main(void){
    //vec3 lightPosition= vec3(0.0,0.0,0.0);
    vec3 lightDir = lightPosition - vec3(ex_worldPosition);
    float dot_product = max(dot(normalize(lightDir), normalize(ex_worldNormal)), 0.0);
    vec4 diffuse = dot_product * vec4( 0.385, 0.647, 0.812, 1.0);
    vec4 ambient = vec4( 0.1, 0.1, 0.1, 1.0);
    out_Color = ambient + diffuse;
}