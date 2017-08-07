
#ifndef _GLMANAGER_H_
#define _GLMANAGER_H_

// Define bitmasks for mouse status, we assume mice have less than 17 buttons


/* Main Header file that contains all the HEADER */
//#include <windows.h>

#include "Ilistener.h"
#include <string>


struct ContextSettings {

	unsigned int flags;
	int majorVersion;
	int minorVersion;
	bool core;
	bool depthBits;
	bool stencilBits;
	bool colorBits;
	bool antiAliasing;

	ContextSettings();
	ContextSettings(bool depth, bool stencil, bool color = true, bool antialiasing = false, unsigned int majorVersion = 3, unsigned int minorVersion = 3);
	const unsigned int getUpdateFlags();


};

struct WindowInfo
{
	std::string name;
	unsigned int width, height;
	bool reshapeble;

	WindowInfo();
	WindowInfo(const std::string & windowName, const unsigned int & width, const unsigned int & height, bool reshapeble = true);
};

class GLManager {

private:

	/*float deltatime;
	
	int oldGlutTime = 0;
	*/
	static unsigned int mouse_status;
	static int _oldGlutTime;
	static IListener * _listener;
	static WindowInfo _windowInfo;
	static ContextSettings _context;


	//static void initGLUT(int * argc, char* argv[]);
	static void displayCallback(void);
	static void reshapeCallback(int width, int height);
	static void keyboardCallback(unsigned char key, int x, int y);
	static void specialCallback(int key, int x, int y);
	static void specialUpCallback(int key, int x, int y);
	static void mouseCallback(int button, int state, int x, int y);
	static void mouseMoveCallback(int x, int y);
	static void closeCallback();
	static void idleCallback(void);
	
	static void initGLEW();

public:

	//GLUT(int * argc, char* argv[]);
	
	
	static bool initGLUT(const WindowInfo & window, const ContextSettings & context);
	static void close();
	static void run();
	static void printOpenGLInfo();
	static void setListener(IListener *& Ilistener);

};



#endif