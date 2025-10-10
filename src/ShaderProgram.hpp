#pragma once

#include "Camera.hpp"
#include "Subject.hpp"
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
#include <iostream>

#pragma once

class ShaderProgram : public Observer
{
    private:
        GLuint Id;
        GLuint vertexShader;
        GLuint fragmentShader;
        Camera* camera;
    public:
        ShaderProgram(const char* vertS, const char* fragS);
        void setProgram()
        {glUseProgram(Id);}
        void resetProgram()
        {glUseProgram(0);}
        void setUniform(const char* name, glm::mat4 mat);
        void setUniform(const char* name, int var);
        void setUniform(const char* name, glm::vec3 matrix);
};