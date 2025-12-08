#include "App.hpp"
#include "Camera.hpp"
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
static void window_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	if(Camera::instance != nullptr)
		Camera::instance->resizeWindow(window, width, height);
}
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

		if(index >= 100)
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

	glfwSetFramebufferSizeCallback(window, window_size_callback);
}

void App::createShaders()
{
	shCr = new ShaderCreator();
	shaders.emplace_back(shCr->createProgramFromFiles("../src/shaders/default.vert","../src/shaders/default.frag"));
	shaders.emplace_back(shCr->createProgramFromFiles("../src/shaders/phong.vert","../src/shaders/phong.frag"));
	shaders.emplace_back(shCr->createProgramFromFiles("../src/shaders/plain.vert","../src/shaders/plain.frag"));
	shaders.emplace_back(shCr->createProgramFromFiles("../src/shaders/default_w.vert","../src/shaders/default.frag"));
	shaders.emplace_back(shCr->createProgramFromFiles("../src/shaders/skybox.vert","../src/shaders/skybox.frag"));
	shaders.emplace_back(shCr->createProgramFromFiles("../src/shaders/whisp.vert","../src/shaders/whisp.frag"));
}

void App::sceneForest()
{
	std::vector<Light*> forest_lights;
	std::vector<PointLight*> whisps_lights;
	for(int i = 0 ; i < 4 ; ++i)
	{
		PointLight* l = new PointLight(glm::vec3(0.0f), glm::vec3(1.0f, 0.5f, 0.05f));
		forest_lights.emplace_back(l);
		whisps_lights.emplace_back(l);
	}
	//FlashLight* fl = new FlashLight(glm::vec3(1.0f));
	//forest_lights.emplace_back(new DirectionLight(glm::vec3(0.0f,-0.1f,0.0f)));
    scenes.emplace_back(new Scene(shaders, forest_lights));

	//scenes.back()->getCamnera()->registerObserver(fl);

	DrawableObject* whisp0 = new DrawableObject(new Model(sphere, sizeof(sphere)),shaders[5],new TransformationComposite({new WhispDynamicTranslation(whisps_lights[0], glm::vec3(17.5f,5.0f,17.5f), glm::vec3(17.5f,5.0f,17.5f), 3.0f),new Scale(glm::vec3(0.2f))}),new Material(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), 32));
	DrawableObject* whisp1 = new DrawableObject(new Model(sphere, sizeof(sphere)),shaders[5],new TransformationComposite({new WhispDynamicTranslation(whisps_lights[1], glm::vec3(17.5f,5.0f,52.5f), glm::vec3(17.5f,5.0f,17.5f), 3.0f),new Scale(glm::vec3(0.2f))}),new Material(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), 32));
	DrawableObject* whisp2 = new DrawableObject(new Model(sphere, sizeof(sphere)),shaders[5],new TransformationComposite({new WhispDynamicTranslation(whisps_lights[2], glm::vec3(52.5f,5.0f,17.5f), glm::vec3(17.5f,5.0f,17.5f), 3.0f),new Scale(glm::vec3(0.2f))}),new Material(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), 32));
	DrawableObject* whisp3 = new DrawableObject(new Model(sphere, sizeof(sphere)),shaders[5],new TransformationComposite({new WhispDynamicTranslation(whisps_lights[3], glm::vec3(52.5f,5.0f,52.5f), glm::vec3(17.5f,5.0f,17.5f), 3.0f),new Scale(glm::vec3(0.2f))}),new Material(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), 32));
	scenes.back()->addObject(whisp0);
	scenes.back()->addObject(whisp1);
	scenes.back()->addObject(whisp2);
	scenes.back()->addObject(whisp3);


    Model* treeModel = new Model(tree, sizeof(tree));
    Material* treeMaterial = new Material(glm::vec3(0.0f, 0.03f, 0.0f), glm::vec3(0.0f,0.5f,0.0f), glm::vec3(0.0f), 32);

	for(int i = 0 ; i < 10 ; ++i)
	{
		for(int j = 0 ; j < 10 ; ++j)
		{
			scenes.back()->addObject(new DrawableObject(treeModel, shaders[1], new TransformationComposite({new Translation(glm::vec3(7 * i, 0.0f, 7 * j))}), treeMaterial));
		}
	}
    
    DrawableObject* floor = new DrawableObject(
        new Model(plain, sizeof(plain), true),
        shaders[1],
        new TransformationComposite({
            new Translation(glm::vec3(35.0f, 0.0f, 35.0f)),
            new Scale(glm::vec3(100.0f))
        }),
        new Material(glm::vec3(0.03f), glm::vec3(0.5f), glm::vec3(0.0f), 1)
    );
    floor->setTexture("../src/textures/grass.png");
    scenes.back()->addObject(floor);
}

void App::scenePlanets()
{
    std::vector<Light*> lights;
	lights.emplace_back(new PointLight(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    scenes.emplace_back(new Scene(shaders, lights));

	Model* sun_model = new Model("planet.obj");
	Model* earth_model = new Model("planet.obj");
	Model* moon_model = new Model("planet.obj");
	sun_model->setTexture("../src/textures/sun.jpg");
    earth_model->setTexture("../src/textures/earth.jpg");
    moon_model->setTexture("../src/textures/moon.jpg");

    Material* sunMat = new Material(glm::vec3(1.0f), glm::vec3(0.0f), glm::vec3(0.0f), 1);
    
    DrawableObject* sun = new DrawableObject(sun_model, shaders[1], new TransformationComposite({
        new Scale(glm::vec3(2.0f))
    }), sunMat);
    scenes.back()->addObject(sun);

    DynamicRotation* earthOrbit = new DynamicRotation(glm::vec3(0.0f, 1.0f, 0.0f),0.5f);

    Material* earthMat = new Material(glm::vec3(0.03f), glm::vec3(0.5f), glm::vec3(0.0f), 1);
    
    DrawableObject* earth = new DrawableObject(earth_model, shaders[1], new TransformationComposite({
        earthOrbit,
        new Translation(glm::vec3(10.0f, 0.0f, 0.0f)),
        new DynamicRotation(glm::vec3(0.0f, 1.0f, 0.0f),2.0f),
        new Scale(glm::vec3(0.7f))
    }), earthMat);
    scenes.back()->addObject(earth);

    Material* moonMat = new Material(glm::vec3(0.03f), glm::vec3(0.5f), glm::vec3(0.0f), 1);

    DrawableObject* moon = new DrawableObject(moon_model, shaders[1], new TransformationComposite({
        earthOrbit,
        new Translation(glm::vec3(10.0f, 0.0f, 0.0f)),
        new DynamicRotation(glm::vec3(0.0f, 1.0f, 0.0f),4.0f),
        new Translation(glm::vec3(2.0f, 0.0f, 0.0f)),
        new Scale(glm::vec3(0.3f))
    }), moonMat);
    scenes.back()->addObject(moon);
}

void App::sceneMole()
{
	//std::vector<std::string> faces = {"../src/textures/posx.jpg","../src/textures/negx.jpg","../src/textures/posy.jpg","../src/textures/negy.jpg","../src/textures/posz.jpg","../src/textures/negz.jpg"};
	std::vector<Light*> mole_lights;
	mole_lights.emplace_back(new DirectionLight(glm::vec3(-0.5f, -1.0f, -0.5f)));
	scenes.emplace_back(new Scene(shaders,mole_lights));
	//scenes.back()->setSkyBox(faces, shaders[4]);
	Model* plainModel0 = new Model(plain,sizeof(plain),true);
	plainModel0->setTexture("../src/textures/grass.png");
	scenes.back()->addObject(new DrawableObject(plainModel0,shaders[1], new TransformationComposite({new Translation(glm::vec3(1.0f,0.0f,1.0f)),new Scale(glm::vec3(2.0f))}),new Material(glm::vec3(0.0f),glm::vec3(0.0f),glm::vec3(0.0f),1)));
	int moleCnt = 100;
	for(int i = 0; i < 3 ; ++i)
	{
		for(int j = 0 ; j < 3 ; ++j)
		{
			Model* sphereModel0 = new Model(sphere,sizeof(sphere));
			TransformationComposite* trc0 = new TransformationComposite({new MoleDynamicTranslation(glm::vec3((float)i,-1.0f,(float)j),glm::vec3((float)i,1.0f,(float)j)),new Scale(glm::vec3(0.2f))});
			DrawableObject* drwobj0 = new DrawableObject(sphereModel0,shaders[1],trc0,new Material(glm::vec3(0.55f,0.24f,0.0f),glm::vec3(0.55f,0.24f,0.0f),glm::vec3(0.1f),32),moleCnt);
			scenes.back()->addObject(drwobj0);
			++moleCnt;
		}
	}
}

void App::sceneFormula()
{
	std::vector<Light*> lights_formula;
	lights_formula.emplace_back(new DirectionLight(glm::vec3(-0.5f,-1.0f,-0.5f)));
	lights_formula.emplace_back(new DirectionLight(glm::vec3(0.5f,0.1f,0.5f)));
	scenes.emplace_back(new Scene(shaders, lights_formula));
	Model* plane = new Model(plain, sizeof(plain), true);
	plane->setTexture("../src/textures/grass.png");
	scenes.back()->addObject(new DrawableObject(plane,shaders[1], new TransformationComposite({new Translation(glm::vec3(0.0f,-1.0f,0.0f)),new Scale(glm::vec3(30.0f))}),new Material(glm::vec3(0.1f),glm::vec3(0.6f),glm::vec3(0.0f),1)));
	
	std::vector<glm::mat4x3> path;
	path.push_back(glm::mat4x3(glm::vec3(0, 0, 0), glm::vec3(5, 0, 5), glm::vec3(10, 0, 5), glm::vec3(10, 0, 0)));
	path.push_back(glm::mat4x3(glm::vec3(10, 0, 0), glm::vec3(10, 0, -5), glm::vec3(5, 0, -5), glm::vec3(0, 0, 0)));
	path.push_back(glm::mat4x3(glm::vec3(0, 0, 0), glm::vec3(-5, 0, 5), glm::vec3(-10, 0, 5), glm::vec3(-10, 0, 0)));
	path.push_back(glm::mat4x3(glm::vec3(-10, 0, 0), glm::vec3(-10, 0, -5), glm::vec3(-5, 0, -5), glm::vec3(0, 0, 0)));

	Model* formulaModel = new Model("formula1.obj");
	TransformationComponent* bezierMove = new BezierTranslation(path, 0.5f);
	TransformationComposite* formulaTransform = new TransformationComposite({bezierMove,new Rotation(glm::vec3(0, -90, 0)),new Scale(glm::vec3(0.1f))});
	scenes.back()->addObject(new DrawableObject(formulaModel,shaders[1],formulaTransform,new Material(glm::vec3(0.1f), glm::vec3(0.8f, 0.1f, 0.1f), glm::vec3(0.2f), 32)));
}

void App::createScenes()
{
	sceneMole();
	sceneForest();
	scenePlanets();
	sceneFormula();

	/*
	//empty scene
	scenes.emplace_back(new Scene(shaders));
	scenes.back()->addObject(new DrawableObject(new Model(sphere,sizeof(sphere)),shaders[3], new TransformationComposite({new Translation(glm::vec3(0.0f))})));
	scenes.back()->addObject(new DrawableObject(new Model(sphere,sizeof(sphere)),shaders[0], new TransformationComposite({new Translation(glm::vec3(3,0,0))})));

	//custom transformation
	scenes.emplace_back(new Scene(shaders));
	scenes.back()->addObject(new DrawableObject(new Model(sphere,sizeof(sphere)),shaders[0], new TransformationComposite({new CustomTransformation()})));
	scenes.back()->addObject(new DrawableObject(new Model(sphere,sizeof(sphere)),shaders[0], new TransformationComposite({new Translation(glm::vec3(3,0,0))})));
	*/

	//login
	std::vector<Light*> lights_hru;
	lights_hru.emplace_back(new DirectionLight(glm::vec3(0.5,-0.7,0.3)));
	lights_hru.emplace_back(new DirectionLight(glm::vec3(-0.5,0.2,-0.3)));
	scenes.emplace_back(new Scene(shaders,lights_hru));
	Model* hru0268Tex = new Model("hru0268-tex.obj");
	hru0268Tex->setTexture("../src/textures/grass.png");
	scenes.back()->addObject(new DrawableObject(hru0268Tex, shaders[1],new TransformationComposite({new Translation(glm::vec3(0))}),new Material(glm::vec3(0.0f),glm::vec3(0.0f),glm::vec3(0.0f),1)));

	//4 balls
	std::vector<Light*> lights_balls;
	//FlashLight* flash = new FlashLight(glm::vec3(-20.5f,5.0f,-14.0f),glm::vec3(-1,0,0),glm::vec3(1.0f,0.1f,0.01f),20);
	lights_balls.emplace_back(new PointLight(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(1.0,0.1,0.01)));
	scenes.emplace_back(new Scene(shaders,lights_balls));

	scenes.back()->addObject(new DrawableObject(new Model(sphere,sizeof(sphere)),shaders[1],new TransformationComposite({new Translation(glm::vec3(-1,0,0)),new Scale(glm::vec3(0.2))}),new Material(glm::vec3(0.1f,0.0f,0.0f),glm::vec3(0.7f,0.0f,0.0f),glm::vec3(1.0f),4),1));
	scenes.back()->addObject(new DrawableObject(new Model(sphere,sizeof(sphere)),shaders[1],new TransformationComposite({new Translation(glm::vec3(0,0,1)),new Scale(glm::vec3(0.2))}),new Material(glm::vec3(0.1f,0.0f,0.0f),glm::vec3(0.7f,0.0f,0.0f),glm::vec3(1.0f),8),2));
	scenes.back()->addObject(new DrawableObject(new Model(sphere,sizeof(sphere)),shaders[1],new TransformationComposite({new Translation(glm::vec3(1,0,0)),new Scale(glm::vec3(0.2))}),new Material(glm::vec3(0.1f,0.0f,0.0f),glm::vec3(0.7f,0.0f,0.0f),glm::vec3(1.0f),16),3));
	scenes.back()->addObject(new DrawableObject(new Model(sphere,sizeof(sphere)),shaders[1],new TransformationComposite({new Translation(glm::vec3(0,0,-1)),new Scale(glm::vec3(0.2))}),new Material(glm::vec3(0.1f,0.0f,0.0f),glm::vec3(0.7f,0.0f,0.0f),glm::vec3(1.0f),32),4));

	//cubes
	std::vector<std::string> faces = {"../src/textures/posx.jpg","../src/textures/negx.jpg","../src/textures/posy.jpg","../src/textures/negy.jpg","../src/textures/posz.jpg","../src/textures/negz.jpg"};
	std::vector<Light*> lights_cubes;
	lights_cubes.emplace_back(new DirectionLight(glm::vec3(-0.5f, -1.0f, -0.7f)));
	scenes.emplace_back(new Scene(shaders,lights_cubes));
	Model* cubeModel0 = new Model("cube.obj");
	cubeModel0->setTexture("../src/textures/wooden_fence.png");
	Model* cubeModel1 = new Model("cube.obj");
	cubeModel1->setTexture("../src/textures/grass.png");
	DrawableObject* cube0 = new DrawableObject(cubeModel0,shaders[1], new TransformationComposite({new Translation(glm::vec3(0.0f))}),new Material(glm::vec3(0.0f),glm::vec3(0.0f),glm::vec3(0.0f),1));
	DrawableObject* cube1 = new DrawableObject(cubeModel1,shaders[1], new TransformationComposite({new Translation(glm::vec3(0.0f,0.0f,3.0f))}),new Material(glm::vec3(0.0f),glm::vec3(0.0f),glm::vec3(0.0f),1));
	scenes.back()->addObject(cube0);
	scenes.back()->addObject(cube1);
	scenes.back()->setSkyBox(faces,shaders[4]);

	//shrek
	std::vector<Light*> lights_shrek;
	lights_shrek.emplace_back(new DirectionLight(glm::vec3(-0.8f)));
	lights_shrek.emplace_back(new DirectionLight(glm::vec3(0.8f)));
	scenes.emplace_back(new Scene(shaders,lights_shrek));
	Model* fiona = new Model("fiona.obj");
	Model* shrek = new Model("shrek.obj");
	Model* toilet = new Model("toilet.obj");
	fiona->setTexture("../src/textures/fiona.png");
	shrek->setTexture("../src/textures/shrek.png");
	toilet->setTexture("../src/textures/toilet.jpg");
	scenes.back()->addObject(new DrawableObject(fiona,shaders[1],new TransformationComposite({new Translation(glm::vec3(2.0f,0.0f,0.0f))}),new Material(glm::vec3(0.0f),glm::vec3(0.0f),glm::vec3(0.0f),1)));
	scenes.back()->addObject(new DrawableObject(shrek,shaders[1],new TransformationComposite({new Translation(glm::vec3(-2.0f,0.0f,0.0f))}),new Material(glm::vec3(0.0f),glm::vec3(0.0f),glm::vec3(0.0f),1)));
	scenes.back()->addObject(new DrawableObject(toilet,shaders[1],new TransformationComposite({new Translation(glm::vec3(0.0f,0.0f,-4.0f))}),new Material(glm::vec3(0.0f),glm::vec3(0.0f),glm::vec3(0.0f),1)));
}

void App::run()
{
	bool prevKey = false;
	int nScenes = scenes.size();
	float currT = 0, lastT = 0, delta = 0;
	while(!glfwWindowShouldClose(window)){
		currT = glfwGetTime();
		delta = currT - lastT;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
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