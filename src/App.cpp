#include "App.hpp"
#include "Scene.hpp"
#include "TransformationComposite.hpp"
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
	float ratio = width / (float)height;
	glViewport(0, 0, width, height);
}

void App::createShaders()
{
	shaders.emplace_back(new ShaderProgram(vert_sh,frag_sh));
}

void App::createModels()
{
	scenes.emplace_back(new Scene(shaders));
	objects.emplace_back(new DrawableObject(new Model(sphere,sizeof(sphere)), shaders[0],new TransformationComposite({new Translation(glm::vec3(0.0f, 0.0f, 0.0f))})));
	scenes[0]->addObject(objects[0]);
}

void App::run()
{
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window)){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		scenes[0]->drawScene();

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}