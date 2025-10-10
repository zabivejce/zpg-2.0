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
#include <vector>
#include "ShaderProgram.hpp"
#include "Model.hpp"
#include "DrawableObject.hpp"

class App{
    private:
    	GLFWwindow* window;
        ShaderProgram* shader0;
        ShaderProgram* shader1;
        DrawableObject* testObject;
        std::vector<ShaderProgram*> shaders;
        std::vector<DrawableObject*> objects;

    public:
        void init();
        void start();
};