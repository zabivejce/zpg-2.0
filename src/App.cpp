#include "App.hpp"
#include "DrawableObject.hpp"
#include "Model.hpp"
#include "ShaderProgram.hpp"

void App::init()
{
	float points_trg[] = {
	0.0f, 0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f
	};
	float points_sqr[] = {
	0.6f, 0.6f, 0.0f,
	0.8f, 0.6f, 0.0f,
	0.8f, 0.8f, 0.0f,
	0.6f, 0.6f, 0.0f,
	0.6f, 0.8f, 0.0f,
	0.8f, 0.8f, 0.0f
	};

	const char* vertS0 =
	"#version 330\n"
	"layout(location=0) in vec3 vp;"
	"out vec3 position;"
	"void main () {"
	"gl_Position = vec4 (vp, 1.0);"
	"position = vp;"
	"}";

	const char* fragS0 =
	"#version 330\n"
	"in vec3 position;"
	"out vec4 frag_colour;"
	"void main () {"
	"frag_colour = vec4 (1.0, 0.0, 0.0, 1.0);"
	"}";

	const char* vertS1 =
	"#version 330\n"
	"layout(location=0) in vec3 vp;"
	"out vec3 position;"
	"void main () {"
	"gl_Position = vec4 (vp, 1.0);"
	"position = vp;"
	"}";

	const char* fragS1 =
	"#version 330\n"
	"in vec3 position;"
	"out vec4 frag_colour;"
	"void main () {"
	"frag_colour = vec4 (0.0, 1.0, 0.0, 1.0);"
	"}";

	//glfwSetErrorCallback(error_callback);
	if (!glfwInit()) {
	   fprintf(stderr, "ERROR: could not start GLFW3\n");
	   exit(EXIT_FAILURE);
	}

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

	shader0 = new ShaderProgram(vertS0,fragS0);
	shader1 = new ShaderProgram(vertS1,fragS1);
	triangle = new DrawableObject(new Model(points_trg, sizeof(points_trg)),shader1);
	square = new DrawableObject(new Model(points_sqr,sizeof(points_sqr)), shader0);
}

void App::start()
{
	while (!glfwWindowShouldClose(window)){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		triangle->draw();
		square->draw();

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}