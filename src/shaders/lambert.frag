#version 330 core

#define MAX 8

in vec4 ex_worldPosition;
in vec3 ex_worldNormal;

out vec4 out_Color;

uniform vec3 eye = vec3(0.0,0.0,3.0);
uniform vec3 lightPosition;


uniform int lightCount;

struct Light {
    int type;
    vec3 position;
    vec3 direction;
    vec3 attenuation;
};

struct Material {
    vec3 ra;
    vec3 rd;
    vec3 rs;
};

uniform Material material;

uniform Light lights[MAX];


void main(void){

    vec4 ambient = vec4( 0.1, 0.1, 0.1, 1.0) * vec4(material.ra,1.0);
    vec4 objectColor = vec4 (0.385 ,0.647 ,0.812 ,1.0);

    vec4 sumDiff = vec4 (0.0,0.0,0.0,0.0);

    for (int i = 0;i<lightCount;i++)
    {
        if (lights[i].type==1)
        {
        float constant = lights[i].attenuation.x;
        float linear = lights[i].attenuation.y;
        float quadratic = lights[i].attenuation.z;

        vec3 lightDir = lights[i].position - vec3(ex_worldPosition);
        float distance = length(lightDir);

        float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));

        
        float dot_product = max(dot(normalize(lightDir), normalize(ex_worldNormal)), 0.0);
        vec4 diffuse = dot_product * vec4(material.rd,1.0);

        sumDiff += diffuse * attenuation;
        }

        else if (lights[i].type==2)
        {
            vec3 lightDir = normalize(-lights[i].direction);
            float dot_product = max(dot(normalize(lightDir), normalize(ex_worldNormal)), 0.0);
            vec4 diffuse = dot_product * vec4(material.rd,1.0);

            sumDiff += diffuse;
        }
    }

    out_Color = ambient + sumDiff;
}