#include "ShaderProgram.hpp"
#include "DirectionLight.hpp"
#include "PointLight.hpp"
#include "SpotLight.hpp"
#include <ostream>

ShaderProgram::ShaderProgram(const char* vertS, const char* fragS)
{
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertS, NULL);
    glCompileShader(vertexShader);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragS, NULL);
    glCompileShader(fragmentShader);
    Id = glCreateProgram();
    glAttachShader(Id, fragmentShader);
    glAttachShader(Id, vertexShader);
    glLinkProgram(Id);

    GLint status;
    glGetProgramiv(Id, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint infoLogLength;
        glGetProgramiv(Id, GL_INFO_LOG_LENGTH, &infoLogLength);
        GLchar *strInfoLog = new GLchar[infoLogLength + 1];
        glGetProgramInfoLog(Id, infoLogLength, NULL, strInfoLog);
        fprintf(stderr, "Linker failure: %s\n", strInfoLog);
        delete[] strInfoLog;
        exit(EXIT_FAILURE);
    }
}

void ShaderProgram::setUniform(const char* name, glm::mat4 matrix)
{
    GLint uniformLocation = glGetUniformLocation(Id, name);
    if (uniformLocation != -1)
        glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(matrix));
    else
        std::cout<< "did not set uniform: " << name << std::endl;
}
void ShaderProgram::setUniform(const char* name, int var)
{
    GLint uniformLocation = glGetUniformLocation(Id, name);
    if (uniformLocation != -1)
        glUniform1i(uniformLocation, var);
    else
        std::cout<< "did not set uniform: " << name << std::endl;
}
void ShaderProgram::setUniform(const char* name, glm::vec3 matrix)
{
    GLint uniformLocation = glGetUniformLocation(Id, name);
    if (uniformLocation != -1)
        glUniform3fv(uniformLocation, 1, glm::value_ptr(matrix));
    else
        std::cout<< "did not set uniform: " << name << std::endl;
}

void ShaderProgram::setLights(std::vector<Light*> lights)
{
    GLint lightCountUni = glGetUniformLocation(Id,"lightCount");
    if(lightCountUni != -1)
        glUniform1i(lightCountUni, lights.size());
    else
        std::cout<< "did not set uniform: lightCount\n";

    for(int i = 0; i < (int)lights.size();++i)
    {
        std::stringstream ss;
        std::string formated;
        switch (lights[i]->getType())
        {
            case 1:     //pointLight
            {
                PointLight* light = static_cast<PointLight*>(lights[i]);
                ss << "lights[" << i << "].position";
                formated = ss.str();
                GLint lightPosUni = glGetUniformLocation(Id,formated.c_str());
                if(lightPosUni != -1)
                    glUniform3fv(lightPosUni, 1, glm::value_ptr(light->getPosition()));
                else
                    std::cout << ":)\n";
                ss = std::stringstream();
                ss << "lights[" << i << "].attenuation";
                formated = ss.str();
                GLint lightAttUni = glGetUniformLocation(Id, formated.c_str());
                if(lightAttUni != -1)
                    glUniform3fv(lightAttUni, 1, glm::value_ptr(light->getAttenuation()));
                else
                    std::cout << ":)\n";
                ss = std::stringstream();
                ss << "lights[" << i << "].type";
                formated = ss.str();
                GLint lightTypeUni = glGetUniformLocation(Id, formated.c_str());
                if(lightTypeUni != 1)
                    glUniform1i(lightTypeUni, light->getType());
                else
                    std::cout << ":)\n";
                break;
            }
            case 2:     //directionLight
            {
                DirectionLight* light = static_cast<DirectionLight*>(lights[i]);
                ss << "lights[" << i << "].direction";
                formated = ss.str();
                GLint lightDirUni = glGetUniformLocation(Id,formated.c_str());
                glUniform3fv(lightDirUni,1,glm::value_ptr(light->getDirection()));
                ss = std::stringstream();
                ss << "lights[" << i << "].type";
                formated = ss.str();
                GLint lightTypeUni = glGetUniformLocation(Id, formated.c_str());
                glUniform1i(lightTypeUni, light->getType());
                break;
            }
            case 3:     //spotLight
            {
                SpotLight* light = static_cast<SpotLight*>(lights[i]);
                ss << "lights[" << i << "].direction";
                formated= ss.str();
                GLint lightDirUni = glGetUniformLocation(Id, formated.c_str());
                glUniform3fv(lightDirUni,1,glm::value_ptr(light->getDirection()));
                ss = std::stringstream();
                ss << "lights[" << i << "].type";
                formated = ss.str();
                GLint lightTypeUni = glGetUniformLocation(Id, formated.c_str());
                glUniform1i(lightTypeUni, light->getType());
                ss = std::stringstream();
                ss << "lights[" << i << "].position";
                formated = ss.str();
                GLint lightPosUni = glGetUniformLocation(Id, formated.c_str());
                glUniform3fv(lightPosUni, 1, glm::value_ptr(light->getPosition()));
                ss = std::stringstream();
                ss << "lights[" << i << "].attenuation";
                formated = ss.str();
                GLint lightAttUni = glGetUniformLocation(Id, formated.c_str());
                glUniform3fv(lightAttUni, 1, glm::value_ptr(light->getAttenuation()));
                ss = std::stringstream();
                ss << "lights[" << i << "].alpha";
                formated = ss.str();
                GLint angleUni = glGetUniformLocation(Id, formated.c_str());
                glUniform1f(angleUni, light->getAngle());
                break;
            }
        }
    }
}