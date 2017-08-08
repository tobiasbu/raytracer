
#include "ogl\glmanager.h"
#include "utils/stringutils.h"

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <iostream>

IListener* GLManager::_listener = NULL;
WindowInfo GLManager::_windowInfo;
ContextSettings GLManager::_context;
int GLManager::_oldGlutTime = 0;
unsigned int GLManager::mouse_status = 0;


WindowInfo::WindowInfo()
{
	this->name = "OpenGL - Freeglut";
	this->width = 300;
	this->height = 300;
	this->reshapeble = true;
}

WindowInfo::WindowInfo(const std::string & windowName, const unsigned int & width, const unsigned int & height, bool reshapeble)
{
	this->name = windowName;
	this->width = width;
	this->height = height;
	this->reshapeble = reshapeble;
}

ContextSettings::ContextSettings()
	:
	majorVersion(3),
	minorVersion(3),
	core(true),
	colorBits(true),
	depthBits(false),
	stencilBits(false),
	antiAliasing(0)
{
	getUpdateFlags();
}

ContextSettings::ContextSettings(bool depth, bool stencil, bool color, bool antialiasing , unsigned int majorVersion, unsigned int minorVersion)
{
	this->colorBits = color;
	this->depthBits = depth;
	this->stencilBits = stencil;
	this->antiAliasing = antialiasing;
	this->majorVersion = majorVersion;
	this->minorVersion = minorVersion;
	this->core = false;
	getUpdateFlags();
}

const unsigned int ContextSettings::getUpdateFlags()
{
	flags = GLUT_DOUBLE;

	if (colorBits)
		flags |= GLUT_RGBA | GLUT_ALPHA;
	if (depthBits)
		flags |= GLUT_DEPTH;
	if (stencilBits)
		flags |= GLUT_STENCIL;
	if (antiAliasing)
		flags |= GLUT_MULTISAMPLE;

	return flags;
}

void GLManager::initGLEW()
{
	glewExperimental = true;

	if (glewInit() == GLEW_OK)
	{

	}
	
	if (glewIsSupported("GL_VERSION_4_5"))
	{
		//std::cout << "GLEW GL_VERSION_4_5 is 4.5\n ";
	}
	else
	{
		//std::cout << " GLEW GL_VERSION_4_5 not supported\n ";
	}

}

/*
void GLManager::initGLUT(int * argc, char* argv[])
{

glutInit(argc, argv);


	glutInitWindowSize(640, 480);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(640 / 2, 480 / 2);
	glutCreateWindow("Simple Raytracer");

	/*resetview(640, 480);
	deltatime = 0;
	oldGlutTime = 0;
	stream.str("");
}*/



bool GLManager::initGLUT(const WindowInfo & window, const ContextSettings & context)
{
	int fakeargc = 1;
	char *fakeargv[] = { "fake", NULL };
	glutInit(&fakeargc, fakeargv);

	if (context.core) 
	{
		glutInitContextVersion(
			context.majorVersion,
			context.minorVersion);
		glutInitContextProfile(GLUT_CORE_PROFILE);
	}
	else 
	{
		glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	}

	glutInitDisplayMode(context.flags);
	//ww = glutGet(GLUT_SCREEN_WIDTH); 
	//wh = glutGet(GLUT_SCREEN_HEIGHT);
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - window.width) / 2, (glutGet(GLUT_SCREEN_HEIGHT) - window.height) / 2);
	//glutInitWindowPosition(window.width / 4, window.height / 4);
	glutInitWindowSize(window.width, window.height);
	glutCreateWindow(window.name.c_str());

	glutIdleFunc(idleCallback);
	glutCloseFunc(closeCallback);
	glutDisplayFunc(displayCallback);
	glutReshapeFunc(reshapeCallback);
	glutKeyboardFunc(keyboardCallback);
	glutSpecialFunc(specialCallback);
	glutMouseFunc(mouseCallback);
	glutPassiveMotionFunc(mouseMoveCallback);
	glutSpecialUpFunc(specialUpCallback);

	

	initGLEW();

	glClearColor(0, 0, 0, 0);
	glDisable(GL_LIGHTING);
	glViewport(0, 0, 1.0, 0.0);

	//cleanup
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,	GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	_windowInfo = window;
	_context = context;

	return true;

}

void GLManager::run() 
{
	glutMainLoop();
}

void GLManager::close() 
{
	glutLeaveMainLoop();
}

void GLManager::setListener(IListener *& Ilistener)
{
	_listener = Ilistener;
}


void GLManager::idleCallback() {

	//do nothing, just redisplay
	

	int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	float deltatime = ((float)timeSinceStart - (float)_oldGlutTime) / 1000.f;
	_oldGlutTime = timeSinceStart;

	if (_listener) 
	{
		_listener->update(deltatime);
	}

	//std::cout << deltatime << std::endl;

	glutPostRedisplay();

}

void GLManager::keyboardCallback(unsigned char key, int x, int y) {

	if (_listener)
	{
		_listener->keyboard(key, x, y);

	}

}

void GLManager::specialCallback(int key, int x, int y)
{
	if (_listener)
	{
		_listener->specialKeyboard(key, x, y);

	}
}

void GLManager::specialUpCallback(int key, int x, int y)
{
	if (_listener)
	{
		_listener->specialKeyboardUp(key, x, y);

	}
}

void GLManager::mouseCallback(int button, int state, int x, int y)
{
	//int b;
	/*switch (button) {
		case GLUT_LEFT_BUTTON:   b = MB_LEFT;   break;
		case GLUT_MIDDLE_BUTTON: b = MB_MIDDLE; break;
		case GLUT_RIGHT_BUTTON:  b = MB_RIGHT;  break;
	};*/

	
	if (_listener)
	{
		_listener->mouse(button, state, x, y);
	}
}

void GLManager::mouseMoveCallback(int x, int y)
{
	if(_listener)
	{
		_listener->mouseMove(x, y);
	}
}

void GLManager::displayCallback()
{
	 //  | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT
	/*glClear(GL_COLOR_BUFFER_BIT);

	//glClearColor(0.25f, 0.25f, 0.25f, 1);

	//std::stringstream str;
	//str << "teste";

	drawText(stream.str());
	
	glutSwapBuffers();*/

	//check for NULL
	if (_listener)
	{
		// clear buffer
		//glClear(_context.flags);
		glClear(GL_COLOR_BUFFER_BIT);

		_listener->beginDraw();
		_listener->draw();

		glutSwapBuffers();

		_listener->endDraw();
	}

}

void GLManager::reshapeCallback(int w, int h) {

	if (_windowInfo.reshapeble == true)
	{
		if (_listener)
		{
			_listener->reshape(w,h,_windowInfo.width,	_windowInfo.height);
		}
		_windowInfo.width = w;
		_windowInfo.height = h;
	}

	/*if (w < 640){
		w = 640;
	}

	if (h < 480) {
		h = 480;
	}

	glutReshapeWindow(w, h);
	resetview(w, h);
	glutPostRedisplay();*/
		// Reset The Current Viewport
}

void GLManager::closeCallback() 
{
	close();
}

void GLManager::printOpenGLInfo()
{
	const unsigned char* renderer = glGetString(GL_RENDERER);
	const unsigned char* vendor = glGetString(GL_VENDOR);
	const unsigned char* version = glGetString(GL_VERSION);

	std::cout << "GLUT:\tVendor : " << vendor << std::endl;
	std::cout << "GLUT:\tRenderer : " << renderer << std::endl;
	std::cout << "GLUT:\tOpenGl version: " << version << std::endl;
}
