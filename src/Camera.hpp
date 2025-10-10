#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>	
#include <vector>

#include <vector>
#include "Observer.hpp"
#include "Subject.hpp"
#include <iostream>
class Camera : public Subject
{
    private:
        std::vector<Observer*> shaders;
        glm::vec3 eye;
        glm::vec3 center;
        glm::vec3 up;

        glm::mat4 view;
        glm::mat4 projection;

        float pitch = 0.0f; // up - down
        float yaw = -90.0f; // left - right

        Camera(glm::vec3 eye, glm::vec3 center, glm::vec3 up);

        int width, height;

        float speed = 0.5f, sens = 0.5f;

    public:

        static Camera* instance;
        static Camera* getInstance(glm::vec3 eye, glm::vec3 center, glm::vec3 up);

        void registerShader(Observer* shader);
        void removeShader(Observer* shader);
        void matrix(float FOV, float nearPlane, float farPlane);
        void controls(GLFWwindow* window);

        void resizeWindow(GLFWwindow* window, int width, int height);

        glm::mat4 getProjection()
        {return projection;}
        glm::mat4 getView()
        {return view;}
        glm::vec3 getEye()
        {return eye;}
        glm::vec3 getDirection()
        {return glm::normalize(center - eye);}
};