#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>  
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Model
{
    private:
        const float* points;
        int arrSize;
        GLuint VAO = 0;
        GLuint VBO = 0;
    public:
        Model(const float* points, int arrSize);
        void drawModel();
};