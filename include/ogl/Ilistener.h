
#ifndef _ILISTENER_H_
#define _ILISTENER_H_



class IListener
{

public:

	//virtual ~IListener() = 0;

	virtual void start() = 0;

	//drawing functions notify
	virtual void beginDraw() = 0;
	virtual void draw() = 0;
	virtual void endDraw() = 0;
	virtual void reshape(int width,	int height,int previous_width,int previous_height) = 0;

	// event - keyboard
	virtual void keyboard(const unsigned char & key, const int & x, const int & y) { }
	virtual void specialKeyboard(int key, int x, int y) {}
	virtual void specialKeyboardUp(int key, int x, int y) {}
	virtual void mouse(const int & button, int state, int x, int y) {}
	virtual void mouseMove(int x, int y) {}

	// update
	virtual void update(const float & deltatime) = 0;

	inline IListener::~IListener()
	{
		//implementation of pure virtual destructor
	}

};

#endif