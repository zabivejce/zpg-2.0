#pragma once
#include "ShaderProgram.hpp"
#include "models/skycube.hpp"
#include <SOIL/SOIL.h>
class SkyBox
{
    private:
        GLuint VAO = 0;
        GLuint VBO = 0;
        GLuint tex;
        ShaderProgram* shader;
    public:
        SkyBox(std::vector<std::string> faces, ShaderProgram* shaderProgram);
        void draw(glm::mat4 projection, glm::mat4 view);
};