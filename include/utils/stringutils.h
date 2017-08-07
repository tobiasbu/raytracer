
#ifndef   _STRINGUTIL_HPP_
#define   _STRINGUTIL_HPP_

#include <string>
#include <vector>

class vec3;
class vec2;
class vec4;

namespace StringUtils {

	std::string convert(bool);
	std::string convert(int);
	std::string convert(unsigned int);
	std::string convert(float);
	std::string convert(double);


	bool parseBool(std::string);
	int parseInt(std::string);
	unsigned int parseUint(std::string);
	float parseFloat(std::string);
	double parseDouble(std::string);


	vec3 parseVec3(std::string);
	vec3 tokensParseVec3(const std::vector<std::string> & tokens, size_t atIndex = 1);
	vec2 tokensParseVec2(const std::vector<std::string> & tokens, size_t atIndex = 1);
	vec4 tokensParseVec4(const std::vector<std::string> & tokens, size_t atIndex = 1, const size_t max = 4);

	void toLower(char * str);
	std::string toLower(std::string &);

	std::string fileExtension(const std::string & filename);
	std::string fileDirectory(const std::string & path);
	std::string fileName(const std::string & path);
	std::string fileNameWithoutExtension(const std::string & path);

	std::string remove(const std::string & str, const std::string & substr);
	std::string removeAtDelimeter(std::string & str, const std::string & delimeter);
	std::string charToString(char * charArray, int size);
	std::string charToString(const unsigned char * charuArray);
	char * stringToChar(std::string);
	

	std::vector<std::string> split(const std::string & str, char delim, bool ignoreEmpty = false);
	
	
	template < class T >
	void tokenize(const std::string& str, T& tokens,
		const std::string& delimiters = " ", bool trimEmpty = false)
	{
		std::string::size_type pos, lastPos = 0, length = str.length();

		typedef T Base;
		typedef typename Base::value_type ValueType;
		typedef typename ValueType::size_type SizeType;


		//using value_type = typename T::value_type;
		//using size_type = typename T::size_type;

		while (lastPos < length + 1)
		{
			pos = str.find_first_of(delimiters, lastPos);
			if (pos == std::string::npos)
			{
				pos = length;
			}

			if (pos != lastPos || !trimEmpty)
				tokens.push_back(ValueType(str.data() + lastPos,
				(SizeType)pos - lastPos));

			lastPos = pos + 1;
		}
	}
};








#endif