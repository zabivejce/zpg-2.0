#include "Camera.hpp"
#include "Subject.hpp"

Camera* Camera::instance = nullptr;

Camera::Camera(glm::vec3 eye, glm::vec3 center, glm::vec3 up)
{
    this->eye = eye;
    this->center = center;
    this->up = up;

    view = glm::mat4(1.0);
    projection = glm::mat4(1.0);
    width = 800;
    height = 600;
}

Camera* Camera::getInstance(glm::vec3 eye, glm::vec3 center, glm::vec3 up)
{
    if(!instance)
        instance = new Camera(eye,center,up);
    return instance;
}

void Camera::registerShader(Observer* shader)
{
    shaders.emplace_back(shader);
}

void Camera::matrix(float FOV, float nearPlane, float farPlane)
{
    float aRation;
    view = glm::lookAt(eye,center,up);

    if(height != 0)
        aRation = width/height;

    projection = glm::perspective(glm::radians(FOV),aRation,nearPlane, farPlane);

    notifyObs();
}

void Camera::resizeWindow(GLFWwindow* window, int width, int height)
{
    this->width = width;
    this->height = height;
    glViewport(0,0,width,height);
}

void Camera::controls(GLFWwindow* window)
{
    glm::vec3 fwd = glm::normalize(center - eye);
    glm::vec3 right = glm::normalize(glm::cross(fwd,up));
    glm::vec3 vert = glm::normalize(up);
    //std::cout << eye.x << ',' << eye.y << ',' << eye.z <<"\n" << center.x << ',' << center.y << ',' << center.z << std::endl;

    if(glfwGetKey(window,GLFW_KEY_W) == GLFW_PRESS)
    {
        eye += fwd * speed;
        center += fwd * speed;
    }
    if(glfwGetKey(window,GLFW_KEY_S) == GLFW_PRESS)
    {
        eye -= fwd * speed;
        center -= fwd * speed;
    }
    if(glfwGetKey(window,GLFW_KEY_D) == GLFW_PRESS)
    {
        eye += right * speed;
        center += right * speed;
    }
    if(glfwGetKey(window,GLFW_KEY_A) == GLFW_PRESS)
    {
        eye -= right * speed;
        center -= right * speed;
    }
    if(glfwGetKey(window,GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        eye += vert * speed;
        center += vert * speed;
    }
    if(glfwGetKey(window,GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        eye -= vert * speed;
        center -= vert * speed;
    }

    if(glfwGetKey(window,GLFW_KEY_UP) == GLFW_PRESS)
        pitch += sens;
    if(glfwGetKey(window,GLFW_KEY_DOWN) == GLFW_PRESS)
        pitch -= sens;
    if(glfwGetKey(window,GLFW_KEY_RIGHT) == GLFW_PRESS)
        yaw += sens;
    if(glfwGetKey(window,GLFW_KEY_LEFT) == GLFW_PRESS)
        yaw -= sens;

    glm::vec3 direct;
    direct.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direct.y = sin(glm::radians(pitch));
    direct.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    center=glm::normalize(direct) + eye;

    glfwSetFramebufferSizeCallback(window,[](GLFWwindow* window,int width,int height){Camera::instance->resizeWindow(window,width,height);});
}