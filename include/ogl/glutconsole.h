
#ifndef _GLUTCONSOLE_H_
#define _GLUTCONSOLE_H_

#include <sstream>
#include <ostream>
#include <string>
#include <vector>
#include <GL/freeglut.h>

//typedef logCallback void * (const std::string &);

class GlutConsole 
{
	float _carretBlinkTime = 0;
	bool _carret = true;
	bool _active = true;
	bool _activeUserInput = true;
	int _editline = -1;
	std::stringstream stream;
	std::vector<std::string> _lines;
	//bool(*_logCallback)(const std::string &) = NULL;

public:

	void resetview(int w, int h);
	void setOrthogonal(const double & right, const double & bottom);
	void drawText(const std::string & str);
	void drawLines();
	void update(const float & dt);
	void draw();
	//void setLogCallback(bool (*callback)(const std::string &));
	void appendLine(std::string str);
	void appendLine(const char * str);
	void userInput(const std::string & in);
	bool getUserInput(std::string & out, bool clear = true);
	void clear();
	void erase(const size_t & index);
	const size_t & count() const;
	//friend std::ostream& operator << (std::ostream &s, const std::string & str);

	void setUserInput(bool flag);

	static void drawText(const std::string text, const float & x, const float & y);

	
	friend std::string & operator>>(std::string & is, GlutConsole & obj)
	{
		obj.appendLine(is);
		return is;

		// read obj from stream
		//if (is.size() > 0) {
		//	*this->appendLine(is);
		//
		//	if (_logCallback)
		//		(*_logCallback)(obj);

		//} else		
		//	is.setstate(std::ios::failbit);
		
	}

	friend const char * operator>>(const char * is, GlutConsole & obj)
	{
		obj.appendLine(std::string(is));
		return is;
	}

	friend const int & operator>>(const int & is, GlutConsole & obj)
	{
		std::stringstream s;
		s << is;
		obj.appendLine(s.str());
		return is;
	}

	/*template<typename T>
	friend std::istream & operator>>(std::istream & is, GlutConsole & obj)
	{
		if (is) {
			std::istreambuf_iterator<char> eos;
			std::string s(std::istreambuf_iterator<char>(is), eos);
			obj.appendLine(std::string(s));
		} else
			is.setstate(std::ios::failbit);
		 
		return is;

		// read obj from stream
		//*if (is.size() > 0) {
		//*this->appendLine(is);

		//if (_logCallback)
		//(*_logCallback)(obj);
//
		//} else
		

	}*/

};

#endif