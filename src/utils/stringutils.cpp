
#include "utils/stringutils.h"

#include "math/vec2.h"
#include "math/vec3.h"
#include "math/vec4.h"

#include <errno.h>
#include <sstream>
#include <algorithm>
#include <stdarg.h>
#include <ctype.h>

#include <stdio.h>

std::string StringUtils::convert(bool value) {

	std::stringstream str;

	if (value)
		str << "true";
	else
		str << "false";

	return str.str();

}

std::string StringUtils::convert(int value) {

	std::stringstream str;
	str << value;
	return str.str();

}

std::string StringUtils::convert(unsigned int value) {

	std::stringstream str;
	str << value;
	return str.str();

}

std::string StringUtils::convert(float value) {

	std::stringstream str;
	str << value;
	return str.str();

}

std::string StringUtils::convert(double value) {

	std::stringstream str;
	str << value;
	return str.str();

}

bool StringUtils::parseBool(std::string value) {

	bool r = false;

	std::transform(value.begin(), value.end(), value.begin(), tolower);

	if (value == "true" || value == "1" || value == "on" || value == "active")
		r = true;
	else if (value == "false" || value == "0" || value == "off" || value == "desactive")
		r = false;


	return r;
}

int StringUtils::parseInt(std::string value) {

	int r = 0;
	std::istringstream str(value);

	str >> r;

	return r;

}

unsigned int StringUtils::parseUint(std::string value) {

	unsigned int r = 0;
	std::istringstream str(value);

	str >> r;

	return r;

}

float StringUtils::parseFloat(std::string value) {

	float r = 0;
	std::istringstream str(value);

	str >> r;

	return r;

}

double StringUtils::parseDouble(std::string value) {

	double r = 0;
	std::istringstream str(value);

	str >> r;

	return r;

}

vec3 StringUtils::parseVec3(std::string value) {

	vec3 r;

	size_t commaOffset = value.find_first_of(',', 0);
	if (commaOffset != std::string::npos)
	{
		float x = parseFloat(value.substr(0, commaOffset));

		// Try to find the next comma
		size_t commaOffset2 = value.find_first_of(',', commaOffset + 1);
		if (commaOffset2 != std::string::npos)
		{
			float y = parseFloat(value.substr(commaOffset + 1, commaOffset2));
			float z = parseFloat(value.substr(commaOffset2 + 1));

			// Now that all 3 values have been parsed, return the Vector3f found

			r.x = x;
			r.y = y;
			r.z = z;
		}
	}


	return r;
}

vec3 StringUtils::tokensParseVec3(const std::vector<std::string> & tokens, size_t atIndex) 
{
	return vec3(tokensParseVec4(tokens, atIndex, 3));
}

vec2 StringUtils::tokensParseVec2(const std::vector<std::string> & tokens, size_t atIndex)
{
	return vec2(tokensParseVec4(tokens, atIndex, 2));
}

vec4 StringUtils::tokensParseVec4(const std::vector<std::string> & tokens, size_t atIndex, const size_t max)
{
	vec4 tmp;

	size_t imax = tokens.size() - atIndex;

	if (imax > max)
		imax = max;

	for (size_t i = atIndex; i < atIndex + imax; i++) {
		std::istringstream tok(tokens[i]);

		

		if (imax == 1) { // scalar 
			float f;
			tok >> f;
			tmp.set(f);
			break;
			//tmp.set(tok.); break;
		}
		else {
			int j = (i - atIndex) % max;
			tok >> tmp[j];
		}
	}

	//std::cout << tmp << std::endl;

	return tmp;
}

void  StringUtils::toLower(char * str)
{
	int i = 0;

	while (str[i] != '\0') { // 
		str[i] = static_cast<char>(tolower(str[i]));
		i++;
	}
}
std::string StringUtils::toLower(std::string & str)
{

	for (std::string::iterator i = str.begin(); i != str.end(); ++i)
		*i = static_cast<char>(tolower(*i));
	return str;
}

std::string StringUtils::fileExtension(const std::string & str)
{
	size_t pos = str.rfind('.');
	if (pos != std::string::npos)
		return str.substr(pos + 1);
	else
		return "";

}

std::string StringUtils::fileDirectory(const std::string & path) 
{
	std::string dir = path;
	bool found = false;

	for (int i = path.length() - 1; i >= 0; i--)
	{
		if (i == 0) {
			dir = "";
			break;
		}
		else {
			if (path[i] == '\\' || path[i] == '/') {
				dir = path.substr(0, i + 1);
				break;
			}
		}
	}

	return dir;
}

std::string StringUtils::fileName(const std::string & path) {

	int i = path.find_last_of('\\');


	if (i != std::string::npos)
		return path.substr(i + 1); // f contains the result :)
	else {
		i = path.find_last_of('/');
		return path.substr(i + 1);
	}

	return path;


}

std::string StringUtils::fileNameWithoutExtension(const std::string & path) {
	std::string fname = fileName(path);

	size_t pos = fname.rfind(("."));
	if (pos == std::string::npos)  //No extension.
		return fname;

	if (pos == 0)    //. is at the front. Not an extension.
		return fname;

	return fname.substr(0, pos);
}

std::string StringUtils::charToString(char * charArray, int size) {

	std::stringstream buffer;

	for (int ch = 0; ch < size; ch++) {
		if (charArray[ch] == '\0') { break; }
		buffer << charArray[ch];

	}

	return buffer.str();
}

std::string StringUtils::charToString(const unsigned char * charuArray) {

	std::stringstream buffer;

	if (charuArray != nullptr) {

		int cidx = 0;

		while (charuArray[cidx] != '\0') {
			buffer << charuArray[cidx];
			cidx++;
		}

		return buffer.str();
	}
	else
		return "";


}

char * StringUtils::stringToChar(std::string str) {

	//const char *cstr = str.c_str();
	char *cstr = new char[str.length() + 1];
	if (errno_t err = strcpy_s(cstr, str.length() + 1, str.c_str()) != 0)
		return NULL;
	else
		return cstr;
}

std::string StringUtils::remove(const std::string & str, const std::string & substr) {


	size_t pos = str.find(substr);

	if (pos == std::string::npos)
		return str;
	else {
		std::string r = str;
		return r.erase(pos, substr.length());
	}
	

}

std::string StringUtils::removeAtDelimeter(std::string & str, const std::string & delimeter) {


	size_t start = str.find(delimeter);

	if (start == std::string::npos)
		return str;
	else {

		//std::string::iterator it = str.begin() + start;

		str.erase(str.begin() + start, str.end());
		return str;
	}

}


std::vector<std::string> StringUtils::split(const std::string & str, char delim, bool ignoreEmpty) {

	std::vector<std::string> tokens;
	std::size_t start = 0, end = 0;
	while ((end = str.find(delim, start)) != std::string::npos) {

		if (ignoreEmpty && end != start) 
			tokens.push_back(str.substr(start, end - start));
		
		start = end + 1;
	}
	if (ignoreEmpty && end != start) {
		tokens.push_back(str.substr(start));
	}
	return tokens;

}
