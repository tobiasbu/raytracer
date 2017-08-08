

//#include "maths_funcs.h"

#include "ogl\glmanager.h" 
#include "manager\applistener.h"

#include <ctime>



int main(int argc, char* argv[]) {



	srand(time(NULL));

	if (GLManager::initGLUT(WindowInfo("Simple Raytracer", 640, 600, true), ContextSettings(false, false,true,false,2,1))) 
	{
		// unique scene
		IListener * appScene = new AppListener();

		//GLManager::printOpenGLInfo();

		GLManager::setListener(appScene);

		appScene->start();

		GLManager::run();
	}


	return 0;

}