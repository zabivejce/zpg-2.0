#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>  
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr


//Include the standard C++ headers  
#include <stdlib.h>
#include <stdio.h>
#include <thread>

#pragma once


class ShaderProgram{
    private:
        GLuint Id;
        GLuint vertexShader;
        GLuint fragmentShader;
    public:
        ShaderProgram(const char* vertS, const char* fragS);
        void setProgram();
};