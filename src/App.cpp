#include "App.hpp"
#include "DrawableObject.hpp"
#include "Model.hpp"
#include "Scene.hpp"
#include "ShaderProgram.hpp"
#include "TransformationComponent.hpp"
#include "TransformationComposite.hpp"
#include "Translation.hpp"
#include <cstddef>

static void error_callback(int error, const char *description) { fputs(description, stderr); }

App::App()
{
	glfwSetErrorCallback(error_callback);
	if(!glfwInit())
	{
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		exit(EXIT_FAILURE);
	}
}

void App::init()
{
	window = glfwCreateWindow(800, 600, "ZPG", NULL, NULL);
	if (!window){
	   glfwTerminate();
	   exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();

	// get version info
	printf("OpenGL Version: %s\n",glGetString(GL_VERSION));
	printf("Using GLEW %s\n", glewGetString(GLEW_VERSION));
	printf("Vendor %s\n", glGetString(GL_VENDOR));
	printf("Renderer %s\n", glGetString(GL_RENDERER));
	printf("GLSL %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	int major, minor, revision;
	glfwGetVersion(&major, &minor, &revision);
	printf("Using GLFW %i.%i.%i\n", major, minor, revision);

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	//float ratio = width / (float)height;
	glViewport(0, 0, width, height);
}

void App::createShaders()
{
	shCr = new ShaderCreator();
	shaders.emplace_back(shCr->createProgramFromFiles("../src/shaders/default.vert","../src/shaders/default.frag"));
	shaders.emplace_back(shCr->createProgramFromFiles("../src/shaders/phong.vert","../src/shaders/phong.frag"));
}

void App::createScenes()
{
	lights.emplace_back(new Light(glm::vec3(2.5f,0.0f,2.5f),glm::vec3(1.0,0.1,0.01)));
	lights.emplace_back(new Light(glm::vec3(37.5f,0.0f,37.5f),glm::vec3(1.0,0.1,0.01)));
	scenes.emplace_back(new Scene(shaders,lights));

	scenes[0]->addObject(new DrawableObject(new Model(sphere,sizeof(sphere)),shaders[1],new TransformationComposite({new Translation(glm::vec3(-1,0,0)),new Scale(glm::vec3(0.2,0.2,0.2))})));
	scenes[0]->addObject(new DrawableObject(new Model(sphere,sizeof(sphere)),shaders[1],new TransformationComposite({new Translation(glm::vec3(0,0,1)),new Scale(glm::vec3(0.2,0.2,0.2))})));
	scenes[0]->addObject(new DrawableObject(new Model(sphere,sizeof(sphere)),shaders[1],new TransformationComposite({new Translation(glm::vec3(1,0,0)),new Scale(glm::vec3(0.2,0.2,0.2))})));
	scenes[0]->addObject(new DrawableObject(new Model(sphere,sizeof(sphere)),shaders[1],new TransformationComposite({new Translation(glm::vec3(0,0,-1)),new Scale(glm::vec3(0.2,0.2,0.2))})));
	
	const int space = 5, size = 8;
	scenes.emplace_back(new Scene(shaders,lights));
	for(int i = 0 ; i < size ; i++)
	{
		for(int j = 0 ; j < size ; j++)
		{
			scenes[1]->addObject(new DrawableObject(new Model(tree,sizeof(tree)), shaders[1],new TransformationComposite({new Translation(glm::vec3(i * space, 0.0f, j * space))})));
			scenes[1]->addObject(new DrawableObject(new Model(bushes,sizeof(bushes)),shaders[1], new TransformationComposite({new Translation(glm::vec3(i * space + 1, 0.0f, j * space + 1))})));
		}
	}
}

void App::run()
{
	glEnable(GL_DEPTH_TEST);
	while(!glfwWindowShouldClose(window)){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		scenes[1]->drawScene();
		scenes[1]->getCamnera()->controls(window);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}