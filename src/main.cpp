#include <iostream>
#include "App.hpp"
int main(void)
{
	App* application = new App;
	application->init();
	application->start();
}