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
#include "Scene.hpp"
#include "Translation.hpp"
#include "models/bushes.hpp"
#include "models/gift.hpp"
#include "models/plain.hpp"
#include "models/sphere.hpp"
#include "models/suzi_flat.hpp"
#include "models/suzi_smooth.hpp"
#include "models/tree.hpp"
#include "shaders/shaders.hpp"

class App{
    private:
    	GLFWwindow* window;
        std::vector<ShaderProgram*> shaders;
        std::vector<DrawableObject*> objects;
        std::vector<Scene*> scenes;

    public:
        App();
        void init();
        void createShaders();
        void createModels();
        void run();
};