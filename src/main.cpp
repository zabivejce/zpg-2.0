#include "App.hpp"
int main(void)
{
	App* application = new App;
	application->init();
	application->createShaders();
	application->createModels();
	application->run();
}