#include "App.hpp"
#include "CustomTransformation.hpp"
#include "DirectionLight.hpp"
#include "DrawableObject.hpp"
#include "Material.hpp"
#include "MoleDynamicTranslation.hpp"
#include "Light.hpp"
#include "Model.hpp"
#include "PointLight.hpp"
#include "Rotation.hpp"
#include "Scene.hpp"
#include "ShaderProgram.hpp"
#include "SpotLight.hpp"
#include "TransformationComponent.hpp"
#include "TransformationComposite.hpp"
#include "Translation.hpp"
#include <GLFW/glfw3.h>
#include <cstddef>
#include <glm/ext/vector_float3.hpp>
#include <glm/fwd.hpp>
#include <vector>

//test private git protoze ho nekdo klonoval visco :)

static void error_callback(int error, const char *description) { fputs(description, stderr); }
static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if(action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT)
	{
		App* app = (App*)glfwGetWindowUserPointer(window); // ziskani instance App
		double pos_x, pos_y;
		glfwGetCursorPos(window, &pos_x, &pos_y);

		int width, height;
        glfwGetFramebufferSize(window, &width, &height);
		int x = (int)pos_x;
		int y = height - pos_y;

		GLuint index = 0;

		glReadPixels(x, y, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);

		printf("Clicked at [%d, %d], Stencil ID: %d\n", x, y, index);

		if(index != 0)
		{
            printf(" -> Vybran objekt s ID: %d\n", index);
			app->removeObjFromActiveScene(index);
        }

		float depth;
		glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
		
		glm::vec3 screenX = glm::vec3(x, y, depth);
		Camera* cam = app->getCamera(); // prasarna :)
		glm::mat4 view = cam->getView();
        glm::mat4 projection = cam->getProjection();
        glm::vec4 viewPort = glm::vec4(0, 0, width, height);

		glm::vec3 pos = glm::unProject(screenX, view, projection, viewPort);

		printf("unProject World Pos: [%f, %f, %f]\n", pos.x, pos.y, pos.z);
	}
}

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
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

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

	glfwSetWindowUserPointer(window, this);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
}

void App::createShaders()
{
	shCr = new ShaderCreator();
	shaders.emplace_back(shCr->createProgramFromFiles("../src/shaders/default.vert","../src/shaders/default.frag"));
	shaders.emplace_back(shCr->createProgramFromFiles("../src/shaders/phong.vert","../src/shaders/phong.frag"));
	shaders.emplace_back(shCr->createProgramFromFiles("../src/shaders/plain.vert","../src/shaders/plain.frag"));
	shaders.emplace_back(shCr->createProgramFromFiles("../src/shaders/default_w.vert","../src/shaders/default.frag"));
	shaders.emplace_back(shCr->createProgramFromFiles("../src/shaders/skybox.vert","../src/shaders/skybox.frag"));
}

void App::createScenes()
{
	std::vector<std::string> faces = {"../src/textures/posx.jpg","../src/textures/negx.jpg","../src/textures/posy.jpg","../src/textures/negy.jpg","../src/textures/posz.jpg","../src/textures/negz.jpg"};
	std::vector<Light*> lights_0;
	lights_0.emplace_back(new PointLight(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(1.0,0.1,0.01)));
	scenes.emplace_back(new Scene(shaders,lights_0));
	//scenes.back()->addObject(new DrawableObject(new Model(sphere,sizeof(sphere)),shaders[1], new TransformationComposite({new Translation(glm::vec3(2.0f,0.0f,2.0f))}),new Material(glm::vec3(1.0f,0.0f,0.0f),glm::vec3(1.0f,.0f,.0f),glm::vec3(0.2f))));
	scenes.back()->setSkyBox(faces, shaders.back());
	//scenes.back()->addObject(new DrawableObject(new Model(sphere,sizeof(sphere)),shaders[0], new TransformationComposite({new Translation(glm::vec3(3,0,0))})));

	/*std::vector<Light*> lights_1;
	scenes.emplace_back(new Scene(shaders,lights_1));
	scenes.back()->addObject(new DrawableObject(new Model(sphere,sizeof(sphere)),shaders[0], new TransformationComposite({new CustomTransformation()})));
	scenes.back()->addObject(new DrawableObject(new Model(sphere,sizeof(sphere)),shaders[0], new TransformationComposite({new Translation(glm::vec3(3,0,0))})));
	*/
	std::vector<Light*> lights_2;
	lights_2.emplace_back(new DirectionLight(glm::vec3(0.5f,-0.7,0.3f)));
	scenes.emplace_back(new Scene(shaders,lights_2));
	Model* plainModel0 = new Model(plain,sizeof(plain),true);
	plainModel0->setTexture("../src/textures/grass.png");
	scenes.back()->addObject(new DrawableObject(plainModel0,shaders[1], new TransformationComposite({new Translation(glm::vec3(1.0f,0.0f,1.0f)),new Scale(glm::vec3(2.0f))}),new Material(glm::vec3(1.0f),glm::vec3(1.0f),glm::vec3(1.0f))));
	int moleCnt = 100;
	for(int i = 0; i < 3 ; ++i)
	{
		for(int j = 0 ; j < 3 ; ++j)
		{
			Model* sphereModel0 = new Model(sphere,sizeof(sphere));
			TransformationComposite* trc0 = new TransformationComposite({new MoleDynamicTranslation(glm::vec3((float)i,-1.0f,(float)j),glm::vec3((float)i,1.0f,(float)j)),new Scale(glm::vec3(0.2f))});
			DrawableObject* drwobj0 = new DrawableObject(sphereModel0,shaders[1],trc0,new Material(glm::vec3(1.0f),glm::vec3(1.0f),glm::vec3(1.0f)),moleCnt);
			scenes.back()->addObject(drwobj0);
			++moleCnt;
		}
	}
	/*
	std::vector<Light*> lights_3;
	lights_3.emplace_back(new DirectionLight(glm::vec3(0.5,-0.7,0.3)));
	lights_3.emplace_back(new DirectionLight(glm::vec3(-0.5,0.2,-0.3)));
	scenes.emplace_back(new Scene(shaders,lights_3));
	Model* hru0268Tex = new Model("hru0268-tex.obj");
	hru0268Tex->setTexture("../src/textures/grass.png");
	scenes.back()->addObject(new DrawableObject(hru0268Tex, shaders[1],new TransformationComposite({new Translation(glm::vec3(0))})));

	std::vector<Light*> lights_4;
	FlashLight* flash = new FlashLight(glm::vec3(-20.5f,5.0f,-14.0f),glm::vec3(-1,0,0),glm::vec3(1.0f,0.1f,0.01f),20);
	lights_0.emplace_back(new PointLight(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(1.0,0.1,0.01)));
	scenes.emplace_back(new Scene(shaders,lights_4));

	scenes.back()->addObject(new DrawableObject(new Model(sphere,sizeof(sphere)),shaders[1],new TransformationComposite({new Translation(glm::vec3(-1,0,0)),new Scale(glm::vec3(0.2,0.2,0.2))}),1));
	scenes.back()->addObject(new DrawableObject(new Model(sphere,sizeof(sphere)),shaders[1],new TransformationComposite({new Translation(glm::vec3(0,0,1)),new Scale(glm::vec3(0.2,0.2,0.2))}),2));
	scenes.back()->addObject(new DrawableObject(new Model(sphere,sizeof(sphere)),shaders[1],new TransformationComposite({new Translation(glm::vec3(1,0,0)),new Scale(glm::vec3(0.2,0.2,0.2))}),3));
	scenes.back()->addObject(new DrawableObject(new Model(sphere,sizeof(sphere)),shaders[1],new TransformationComposite({new Translation(glm::vec3(0,0,-1)),new Scale(glm::vec3(0.2,0.2,0.2))}),4));


	const int space = 5, size = 8;
	std::vector<Light*> lights_5;
	lights_1.emplace_back(new PointLight(glm::vec3(2.5f,0.0f,2.5f),glm::vec3(1.0,0.1,0.01)));
	lights_1.emplace_back(new PointLight(glm::vec3(37.5f,0.0f,37.5f),glm::vec3(1.0,0.1,0.01)));
	//lights_1.emplace_back(new DirectionLight(glm::vec3(0.0f,-0.1f,0.0f)));
	scenes.emplace_back(new Scene(shaders,lights_5));
	Model* plainModel1 = new Model(plain,sizeof(plain),true);
	plainModel1->setTexture("../src/textures/grass.png");
	scenes.back()->addObject(new DrawableObject(plainModel1,shaders[2],new TransformationComposite({new Translation(glm::vec3(17.5f,0.0f,17.5f)),new Scale(glm::vec3(20.0f))})));
	for(int i = 0 ; i < size ; i++)
	{
		for(int j = 0 ; j < size ; j++)
		{
			scenes.back()->addObject(new DrawableObject(new Model(tree,sizeof(tree)), shaders[1],new TransformationComposite({new Translation(glm::vec3(i * space, 0.0f, j * space))})));
			scenes.back()->addObject(new DrawableObject(new Model(bushes,sizeof(bushes)),shaders[1], new TransformationComposite({new Translation(glm::vec3(i * space + 1, 0.0f, j * space + 1))})));
		}
	}

	std::vector<Light*> lights_7;
	lights_3.emplace_back(new DirectionLight(glm::vec3(-0.5f, -1.0f, -0.7f)));
	scenes.emplace_back(new Scene(shaders,lights_7));
	Model* cubeModel0 = new Model("cube.obj");
	cubeModel0->setTexture("../src/textures/wooden_fence.png");
	Model* cubeModel1 = new Model("cube.obj");
	cubeModel1->setTexture("../src/textures/grass.png");
	DrawableObject* cube0 = new DrawableObject(cubeModel0,shaders[1], new TransformationComposite({new Translation(glm::vec3(0.0f))}));
	DrawableObject* cube1 = new DrawableObject(cubeModel1,shaders[1], new TransformationComposite({new Translation(glm::vec3(0.0f,0.0f,3.0f))}));
	scenes.back()->addObject(cube0);
	scenes.back()->addObject(cube1);

	std::vector<Light*> lights_8;
	lights_8.emplace_back(new DirectionLight(glm::vec3(-0.8f)));
	lights_8.emplace_back(new DirectionLight(glm::vec3(0.8f)));
	scenes.emplace_back(new Scene(shaders,lights_8));
	Model* fiona = new Model("fiona.obj");
	Model* shrek = new Model("shrek.obj");
	Model* toilet = new Model("toilet.obj");
	fiona->setTexture("../src/textures/fiona.png");
	shrek->setTexture("../src/textures/shrek.png");
	toilet->setTexture("../src/textures/toilet.jpg");
	scenes.back()->addObject(new DrawableObject(fiona,shaders[1],new TransformationComposite({new Translation(glm::vec3(2.0f,0.0f,0.0f))})));
	scenes.back()->addObject(new DrawableObject(shrek,shaders[1],new TransformationComposite({new Translation(glm::vec3(-2.0f,0.0f,0.0f))})));
	scenes.back()->addObject(new DrawableObject(toilet,shaders[1],new TransformationComposite({new Translation(glm::vec3(0.0f,0.0f,-4.0f))})));*/
}

void App::run()
{
	bool prevKey = false;
	int nScenes = scenes.size();
	float currT = 0, lastT = 0, delta = 0;
	while(!glfwWindowShouldClose(window)){
		currT = glfwGetTime();
		delta = currT - lastT;
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
		
		scenes[activeScene]->update(delta);
		scenes[activeScene]->drawScene();
		scenes[activeScene]->getCamnera()->controls(window);

		glfwPollEvents();
		glfwSwapBuffers(window);
		lastT = currT;
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}