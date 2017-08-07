
#include "ogl\glutconsole.h"

/*void GlutConsole::setLogCallback(bool(*callback)(const std::string &)) {

	_logCallback = callback;

}*/

void GlutConsole::appendLine(const char * str) 
{
	std::string s(str);
	_lines.push_back(s);
}

void GlutConsole::appendLine(std::string str)
{
	
	_lines.push_back(str);


	/*if (_logCallback) {
		_logCallback(str);
	}*/

}

void GlutConsole::setUserInput(bool flag)
{
	_activeUserInput = flag;
}

void  GlutConsole::clear() 
{
	_lines.clear();
}

const size_t & GlutConsole::count() const
{
	return _lines.size();
}

void GlutConsole::erase(const size_t & index) {

	if (index < _lines.size())
		_lines.erase(_lines.begin() + index);

}

void  GlutConsole::userInput(const std::string & in)
{
	if (_editline <= -1) {
		_lines.push_back(in);
		_editline = _lines.size() - 1;
	}
	else {
		_lines[_editline] = in;
	}

	_carret = true;
	_carretBlinkTime = 0;

}

bool GlutConsole::getUserInput(std::string & out, bool clear) {
	if (_editline <= -1) {
		return false;
	}
	else {
		if (_lines[_editline].size() > 0)
			out = _lines[_editline];
		else
			return false;

		if (clear) {
			_lines.erase(_lines.begin() + _editline);
			_editline = -1;
		}
		return true;
	}
}

void GlutConsole::update(const float & dt)
{
	if (_active && _activeUserInput)
	{
		_carretBlinkTime += dt / 0.65f;

		if (_carretBlinkTime >= 1) {
			_carret = !_carret;
			_carretBlinkTime = 0;
		}
	}
}

void GlutConsole::draw()
{
	if (_active)
	{
		GLfloat x = 18;
		GLfloat w = 0;
		GLfloat y = 32;
		size_t j = 0;
		size_t i = 0;
		size_t cCount = 0;;

		glRasterPos2f(x, y);
		//int countN = 0;

		for (; i < _lines.size(); i++)
		{
			cCount = 0;

			for (j = 0; j < _lines[i].size(); j++) {

				cCount++;
				bool skip = false;
				bool draw = true;

				if (_lines[i][j] == '\n' && _lines[i][j] != '//') {
					skip = true;
					draw = false;
				} else
					w += 9;

				if (cCount > 64) {
					skip = true;
					draw = true;
				}
				
				if (skip) {
					x = 18;
					y += 16;
					cCount = 0;
					w = 0;
					glRasterPos2f(x, y);
				}

				if (draw) {
					glutBitmapCharacter(GLUT_BITMAP_9_BY_15, _lines[i][j]);
				}
				
			}

			//w = _lines[i].size() * 9;
			//glRasterPos2f(x, y + (16 * i));
		}

		if (_carret && _activeUserInput) {
			glRasterPos2f(x + w, y /*+ (16 * countN)*/);
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '|');
		}
	}
}

void GlutConsole::drawText(const std::string text, const float & x, const float & y) 
{
	GLfloat xx = x;
	GLfloat yy = y;

	glRasterPos2f(x, y);

	for (int i = 0; i < text.size(); i++) {

		if (text[i] == '\n' && text[i] != '//') {
			xx = x;
			yy += 16;
			glRasterPos2f(xx, yy);
		} else {
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, text[i]);
		}

	}
}


/*std::string str = stream.str();
std::string sub = sub;
int countN = 0;
GLfloat w = 9 * sub.size();
GLfloat x = 18;
GLfloat y = 32;

glRasterPos2f(x, y);

for (size_t i = 0; i < str.size(); i++)	{
if (sub.size() > 0) {
if (str[i] == '\n' && sub[i] != '//') {

int index = i;

if (index > sub.size())
index = sub.size() - 1;

if (index > 0) {
sub.erase(0, index);


}
//if (sub.size() >= 1) {
countN++;
glRasterPos2f(x, y + (16 * countN));
if (countN > 0) {

w = 9 * sub.size();
}
//}
//i++;
}
else {
glutBitmapCharacter(GLUT_BITMAP_9_BY_15, str[i]);
}
}


}*/

/*
void GLManager::resetview(int w, int h) {

glViewport(0, 0, w, h);
setOrthogonal(w, h);

}

void GLManager::setOrthogonal(const double & right, const double & bottom)
{
glLoadIdentity();
glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
glPushMatrix();										// Store The Projection Matrix
glLoadIdentity();									// Reset The Projection Matrix
gluOrtho2D(0.0, right, bottom, 0.0);			// Set Up An Ortho Screen
glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
glPushMatrix();										// Store The Modelview Matrix
glLoadIdentity();									// Reset The Modelview Matrix
}

void GLManager::drawText(const std::string & str)
{
std::string sub = str; // = "Simple Raytracer";

int countN = 0;
GLfloat w = 9 * sub.size();
GLfloat x = 18;
GLfloat y = 32;

glRasterPos2f(x, y);

for (size_t i = 0; i < str.size(); i++)	{
if (sub.size() > 0) {
if (str[i] == '\n' && sub[i] != '//') {

int index = i;

if (index > sub.size())
index = sub.size() - 1;

if (index > 0) {
sub.erase(0, index);


}
//if (sub.size() >= 1) {
countN++;
glRasterPos2f(x, y + (16 * countN));
if (countN > 0) {

w = 9 * sub.size();
}
//}
//i++;
}
else {
glutBitmapCharacter(GLUT_BITMAP_9_BY_15, str[i]);
}
}

}
if (carret) {
//glRasterPos2f(x + w, y + (16 * countN));
glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '|');
}
}
*/