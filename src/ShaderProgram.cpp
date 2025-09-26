#include "ShaderProgram.hpp"

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
    }
}

void ShaderProgram::setProgram()
{
    glUseProgram(Id);
}