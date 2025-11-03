#version 450 core
#define MAX_LIGHTS 4

in vec4 ex_worldPosition;
in vec3 ex_worldNormal;
out vec4 fragColor;

uniform vec3 eye = vec3(0.0,0.0,3.0);

uniform int lightCount;

struct Light
{
    int type;
    vec3 position;
    vec3 direction;
    vec3 attenuation;
    float alpha;
};
uniform Light lights[MAX_LIGHTS];

void main (void)
{
    vec4 ambient = vec4(0.1,0.1,0.1,1.0);

    vec3 norm = normalize(ex_worldNormal);

    vec3 viewDir = normalize(eye - vec3(ex_worldPosition));

    vec4 sumDiff = vec4(0.0, 0.0, 0.0, 0.0);
    vec4 sumSpec = vec4(0.0, 0.0, 0.0, 0.0);

    vec4 objectColor = vec4 (0.385 ,0.647 ,0.812 ,1.0);

    for(int i = 0; i < lightCount; ++i)
    {
        if(lights[i].type == 1)         //pointLight
        {
            float constant = lights[i].attenuation.x;
            float linear = lights[i].attenuation.y;
            float quadratic = lights[i].attenuation.z;

            vec3 lightVec = lights[i].position - vec3(ex_worldPosition);
            vec3 lightDir = normalize(lightVec);

            float distance = length(lightVec);

            vec3 reflectDir = reflect(-lightDir, norm);

            float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));

            float spec = 0.0; 
            if(dot(norm,lightDir) >= 0)
                spec = pow(max(dot(viewDir, normalize(reflectDir)), 0.0), 32);

            float diffuse = max(dot(norm,lightDir), 0.0);
            sumDiff += (diffuse * attenuation) * vec4(1.0,1.0,1.0,1.0);
            sumSpec += (spec * attenuation) * vec4(1.0,1.0,1.0,1.0);
        }
        else if(lights[i].type == 2)    //directionLight
        {
            vec3 lightDir = normalize(-lights[i].direction);

            vec3 reflectDir = reflect ( -lightDir , norm );
            

            float diffuse = max(dot(norm,normalize(lightDir)),0.0);
            vec4 diff = diffuse * vec4(1.0,1.0,1.0,1.0);

            float spec = 0.0;
            if(dot(norm,lightDir) >= 0)
                spec = pow(max(dot(viewDir, normalize(reflectDir)), 0.0), 32);

            sumDiff += diff * objectColor;
            sumSpec += spec;
        }
        else if(lights[i].type == 3)    //spotLight
        {
            vec3 lightDir = normalize(lights[i].position - vec3(ex_worldPosition));
            float theta = dot(lightDir, normalize(-lights[i].direction));

            if(theta > lights[i].alpha)
            {
                float constant = lights[i].attenuation.x;
                float linear = lights[i].attenuation.y;
                float quadratic = lights[i].attenuation.z;

                float distance = length(lights[i].position - vec3(ex_worldPosition));
                float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));

                float diffuse = max(dot(norm, lightDir), 0.0);
                vec4 diff = diffuse * vec4(1.0, 1.0, 1.0, 1.0);

                vec3 reflectDir = reflect(-lightDir, norm);
                float spec = 0.0;
                if(dot(norm, lightDir) >= 0.0)
                    spec = pow(max(dot(viewDir, normalize(reflectDir)), 0.0), 32);

                sumDiff += diff * objectColor * attenuation;
                sumSpec += spec * attenuation;
            }
        }
    }
    fragColor = ambient + (sumDiff * objectColor) + (sumSpec * vec4(1.0,1.0,1.0,1.0));
};