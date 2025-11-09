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
#include <iostream>
#include "libs/tiny_obj_loader.h"
#include <SOIL/SOIL.h>

class Model
{
    private:
        const float* points;
        int vertexCount;
        GLuint VAO = 0;
        GLuint VBO = 0;
        GLuint img;
        bool haveTex = false;
    public:
        Model(const float* points, int arrSize);
        Model(const float* pts, int arrS, bool tex);
        Model(const char* mdlPath);
        void drawModel();
        void setTexture(const char* path);
        bool doHaveTexture()
        {return haveTex;}
};