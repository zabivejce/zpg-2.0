# version 330 core

#define MAX 8


in vec4 ex_worldPosition;
in vec3 ex_worldNormal;
out vec4 fragColor;
in vec2 TexCoord;


uniform sampler2D ourTexture;
uniform vec3 eye = vec3(0.0,0.0,3.0);
uniform vec3 lightPosition;

uniform int lightCount;
uniform int hasTexture;

struct Light {
    int type;
    vec3 position;
    vec3 direction;
    vec3 attenuation;
    float alpha;
};

struct Material {
    vec3 ra;
    vec3 rd;
    vec3 rs;
};


uniform Material material;
uniform Light lights[MAX];


void main (void)
{
    vec4 ambient = vec4( 0.1, 0.1, 0.1, 1.0) * vec4(material.ra,1.0);

    vec4 objectColor;

    if (hasTexture==0)
    {
        objectColor = vec4 (material.rd ,1.0);
    }
    else
    {
        objectColor = texture(ourTexture, TexCoord);
    }
    

    vec3 norm = normalize( ex_worldNormal );
    vec3 viewDir = normalize(eye - vec3(ex_worldPosition));

    vec4 sumDiff = vec4 (0.0,0.0,0.0,0.0);
    vec4 sumSpec = vec4 (0.0,0.0,0.0,0.0);

    for (int i = 0; i<lightCount;i++)
    {
        if (lights[i].type==1)
        {
            float constant = lights[i].attenuation.x;
            float linear = lights[i].attenuation.y;
            float quadratic = lights[i].attenuation.z;

            vec3 lightDir = lights[i].position - vec3(ex_worldPosition);
            float distance = length(lightDir);

            vec3 reflectDir = reflect ( -lightDir , norm );

            float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));

            float spec = pow(max(dot(viewDir, normalize(reflectDir)), 0.0), 32);
            if (dot(norm,lightDir) < 0)
            {
                spec = 0.0;
            }

            float diffuse = max(dot(norm,normalize(lightDir)),0.0);
            vec4 diff = diffuse * vec4(1.0,1.0,1.0,1.0);
            sumDiff += diff * objectColor * attenuation;
            sumSpec += spec * attenuation * vec4 (material.rs,1.0); 
        }
        else if (lights[i].type==2)
        {
            vec3 lightDir = normalize(-lights[i].direction);

            vec3 reflectDir = reflect ( -lightDir , norm );
            

            float diffuse = max(dot(norm,normalize(lightDir)),0.0);
            vec4 diff = diffuse * vec4(1.0,1.0,1.0,1.0);

            float spec = pow(max(dot(viewDir, normalize(reflectDir)), 0.0), 32);
            if (dot(norm,lightDir) < 0)
            {
                spec = 0.0;
            }

            sumDiff += diff * objectColor  ;
            sumSpec += spec *  vec4 (material.rs,1.0); 
        }
        else if (lights[i].type==3)
        {
            vec3 lightDir = normalize(lights[i].position - vec3(ex_worldPosition));
            float theta = dot(lightDir, normalize(-lights[i].direction)); // dot(l,f)

            if (theta>lights[i].alpha)
            {
                float constant = lights[i].attenuation.x;
                float linear = lights[i].attenuation.y;
                float quadratic = lights[i].attenuation.z;

                float distance = length(lights[i].position - vec3(ex_worldPosition));
                float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));

                float diffuse = max(dot(norm, lightDir), 0.0);
                vec4 diff = diffuse * vec4(1.0, 1.0, 1.0, 1.0);

                vec3 reflectDir = reflect(-lightDir, norm);
                float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
                if (dot(norm, lightDir) < 0.0) {
                    spec = 0.0;
                }
                sumDiff += diff * objectColor * attenuation;
                sumSpec += spec * attenuation * vec4 (material.rs,1.0);
            }
        }
    }
    if (hasTexture==1)
    {
        fragColor = texture(ourTexture, TexCoord) * (ambient + sumDiff + sumSpec);
    }
    else
    {
        fragColor = ambient + sumDiff + sumSpec;
    }
    
}