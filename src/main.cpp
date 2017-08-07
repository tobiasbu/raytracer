

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

	//glutInit(&argc, argv);

	//App app;
	//app.execute();

	

	// img;

	//img.saveBMP("output.bmp");

	
	
	//t.m[5] = 135153;


	//std::cout << std::numeric_limits<float>().has_infinity << std::endl;

	//std::cout << std::numeric_limits<float>().infinity() << std::endl;

	//system("pause");
	//while (window. ) {

	//}

	return 0;

}