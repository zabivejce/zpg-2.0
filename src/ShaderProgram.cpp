#include "ShaderProgram.hpp"
#include <cstdlib>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>

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
        std::cout<< "did not set uniform\n";
}
void ShaderProgram::setUniform(const char* name, int var)
{
    GLint uniformLocation = glGetUniformLocation(Id, name);
    if (uniformLocation != -1)
        glUniform1i(uniformLocation, var);
    else
        std::cout<< "did not set uniform\n";
}
void ShaderProgram::setUniform(const char* name, glm::vec3 matrix)
{
    GLint uniformLocation = glGetUniformLocation(Id, name);
    if (uniformLocation != -1)
        glUniform3fv(uniformLocation, 1, glm::value_ptr(matrix));
    else
        std::cout<< "did not set uniform\n";
}

void ShaderProgram::setLights(std::vector<Light*> lights)
{
    GLint lightCountUni = glGetUniformLocation(Id,"lightCount");
    if(lightCountUni != -1)
        glUniform1i(lightCountUni, lights.size());
    else
        std::cout<< "did not set uniform\n";

    //std::cout << lights.size() << std::endl;
    for(int i = 0; i < lights.size();++i)
    {
        std::stringstream ss;
        std::string formated;

        ss << "lights[" << i << "].position";
        formated = ss.str();
        GLint lightPosUni = glGetUniformLocation(Id,formated.c_str());
        if(lightPosUni != -1)
            glUniform3fv(lightPosUni, 1, glm::value_ptr(lights[i]->getPosition()));
        else
            std::cout << ":)\n";
        ss = std::stringstream();
        ss << "lights[" << i << "].attenuation";
        formated = ss.str();
        GLint lightDiffUni = glGetUniformLocation(Id, formated.c_str());
        if(lightDiffUni != -1)
            glUniform3fv(lightDiffUni, 1, glm::value_ptr(lights[i]->getAttenuation()));
        else
            std::cout << ":)\n";
    }
}