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
#include "ShaderCreator.hpp"
#include "Model.hpp"
#include "DrawableObject.hpp"
#include "Scene.hpp"
#include "Translation.hpp"
#include "Scale.hpp"

#include "models/bushes.hpp"
#include "models/gift.hpp"
#include "models/plain.hpp"
#include "models/sphere.hpp"
#include "models/suzi_flat.hpp"
#include "models/suzi_smooth.hpp"
#include "models/tree.hpp"

class App{
    private:
    	GLFWwindow* window;
        ShaderCreator* shCr;
        std::vector<ShaderProgram*> shaders;
        std::vector<Scene*> scenes;

    public:
        App();
        void init();
        void createShaders();
        void createScenes();
        void run();
};