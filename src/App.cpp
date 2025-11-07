#include "App.hpp"
#include "DirectionLight.hpp"
#include "DrawableObject.hpp"
#include "Light.hpp"
#include "Model.hpp"
#include "PointLight.hpp"
#include "Scene.hpp"
#include "ShaderProgram.hpp"
#include "SpotLight.hpp"
#include "TransformationComponent.hpp"
#include "TransformationComposite.hpp"
#include "Translation.hpp"
#include <GLFW/glfw3.h>
#include <cstddef>

//test private git protoze ho nekdo klonoval visco :)

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
	std::vector<Light*> lights_0;
	FlashLight* flash = new FlashLight(glm::vec3(-20.5f,5.0f,-14.0f),glm::vec3(-1,0,0),glm::vec3(1.0f,0.1f,0.01f),20);
	lights_0.emplace_back(new PointLight(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(1.0,0.1,0.01)));
	scenes.emplace_back(new Scene(shaders,lights_0));

	scenes[0]->addObject(new DrawableObject(new Model(sphere,sizeof(sphere)),shaders[1],new TransformationComposite({new Translation(glm::vec3(-1,0,0)),new Scale(glm::vec3(0.2,0.2,0.2))})));
	scenes[0]->addObject(new DrawableObject(new Model(sphere,sizeof(sphere)),shaders[1],new TransformationComposite({new Translation(glm::vec3(0,0,1)),new Scale(glm::vec3(0.2,0.2,0.2))})));
	scenes[0]->addObject(new DrawableObject(new Model(sphere,sizeof(sphere)),shaders[1],new TransformationComposite({new Translation(glm::vec3(1,0,0)),new Scale(glm::vec3(0.2,0.2,0.2))})));
	scenes[0]->addObject(new DrawableObject(new Model(sphere,sizeof(sphere)),shaders[1],new TransformationComposite({new Translation(glm::vec3(0,0,-1)),new Scale(glm::vec3(0.2,0.2,0.2))})));


	const int space = 5, size = 8;
	std::vector<Light*> lights_1;
	lights_1.emplace_back(new PointLight(glm::vec3(2.5f,0.0f,2.5f),glm::vec3(1.0,0.1,0.01)));
	lights_1.emplace_back(new PointLight(glm::vec3(37.5f,0.0f,37.5f),glm::vec3(1.0,0.1,0.01)));
	scenes.emplace_back(new Scene(shaders,lights_1));
	for(int i = 0 ; i < size ; i++)
	{
		for(int j = 0 ; j < size ; j++)
		{
			scenes[1]->addObject(new DrawableObject(new Model(tree,sizeof(tree)), shaders[1],new TransformationComposite({new Translation(glm::vec3(i * space, 0.0f, j * space))})));
			scenes[1]->addObject(new DrawableObject(new Model(bushes,sizeof(bushes)),shaders[1], new TransformationComposite({new Translation(glm::vec3(i * space + 1, 0.0f, j * space + 1))})));
		}
	}

	std::vector<Light*> lights_2;
	lights_2.emplace_back(new DirectionLight(glm::vec3(-0.3f,-1.0f,-0.7f)));
	lights_2.emplace_back(new DirectionLight(glm::vec3(0.7f,1.0f,0.3f)));
	scenes.emplace_back(new Scene(shaders,lights_2));
	scenes[2]->addObject(new DrawableObject(new Model("formula1.obj"),shaders[1], new TransformationComposite({new Translation(glm::vec3(0.0f))})));

	std::vector<Light*> lights_3;
	lights_3.emplace_back(new DirectionLight(glm::vec3(-0.1f, -1.0f, -0.1f)));
	scenes.emplace_back(new Scene(shaders,lights_3));
	scenes[3]->addObject(new DrawableObject(new Model("cube.obj"),shaders[1], new TransformationComposite({new Translation(glm::vec3(0.0f))})));
}

void App::run()
{
	bool prevKey = false;
	int nScenes = scenes.size();
	int activeScene = 0;
	glEnable(GL_DEPTH_TEST);
	while(!glfwWindowShouldClose(window)){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
    	if((glfwGetKey(window,GLFW_KEY_TAB) == GLFW_PRESS) && prevKey == false)
		{
			prevKey = true;
			if(activeScene + 1 == nScenes)
				activeScene = 0;
			else
				++activeScene;
		}
		if((glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE))
			prevKey = false;

		scenes[activeScene]->drawScene();
		scenes[activeScene]->getCamnera()->controls(window);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}