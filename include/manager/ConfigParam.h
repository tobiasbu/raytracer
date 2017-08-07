
#ifndef _CONFIGPARAM_H_
#define _CONFIGPARAM_H_

#include <string>
#include <sstream>
#include "math/maths.h"

// from imgui
static bool SingleStringGetter(void* data, int idx, const char** out_text)
{
	// FIXME-OPT: we could pre-compute the indices to fasten this. But only 1 active combo means the waste is limited.
	const char* items_separated_by_zeros = (const char*)data;
	int items_count = 0;
	const char* p = items_separated_by_zeros;
	while (*p)
	{
		if (idx == items_count)
			break;
		p += strlen(p) + 1;
		items_count++;
	}
	if (!*p)
		return false;
	if (out_text)
		*out_text = p;
	return true;
}

struct ConfigParamBase
{
	//
	std::string name;
	int type;

	ConfigParamBase(const std::string & name)
	{
		//this->value = value;
		this->name = name;
		type = -1;
	}

	virtual std::string toString() = 0;

	/*template<>
	ConfigParam<int>(const std::string & name, int value)
	:
	ConfigParam<int>(name,value)
	{

	}

	*/
};

template<typename T>
struct ConfigParam : public ConfigParamBase
{
	typedef T value_type;
	value_type value;


	ConfigParam(const std::string & name, value_type value)
		:
		ConfigParamBase(name)
	{
		this->value = value;
		type = 1;
	}

	void set(value_type value)
	{
		this->value = value;
	}

	//template<typename T>
	virtual std::string toString()
	{
		if (type == 1)
		{
			if (value)
				return "ON";
			else
				return "OFF";
		}

		std::stringstream s;

		s << value;

		return s.str();
	}

	template<typename T>
	std::string toString()
	{



		if (type == 1)
		{
			if (value)
				return "ON";
			else
				return "OFF";
		}
		else

			std::stringstream s;

		s << value;

		return s.str();

	}

	template<>
	std::string toString<bool>()
	{

		if (value)
			return "ON";
		else
			return "OFF";

	}



};


/*
template<typename bool>
std::string ConfigParam::toString()
{

if (value)
return "ON";
else
return "OFF";

}*/

template<>
struct ConfigParam<int> : public ConfigParamBase
{
	typedef int value_type;
	value_type value;
	value_type min;
	value_type max;

	ConfigParam(const std::string & name, value_type value, value_type min = 0, value_type max = 0)
		:
		ConfigParamBase(name)
	{
		this->min = min;
		this->max = max;
		this->value = value;
		type = 2;
	}

	int set(value_type value)
	{
		if (min != max)
			value = Math::clampint(value, min, max);

		this->value = value;

		return value;
	}

	std::string toString()
	{
		std::stringstream s;

		s << value;

		return s.str();
	}

};

template<>
struct ConfigParam<const char *> : public ConfigParamBase
{
	typedef const char * value_type;
	value_type values;
	value_type currentValue;
	int current;
	int size;

	ConfigParam(const std::string & name, value_type values, int size = 1, int current = 0)
		:
		ConfigParamBase(name)
	{
		this->current = current;
		this->size = size;
		this->values = values;
		type = 3;

		set(this->current);
	}

	void set(int value)
	{


		this->current = Math::clampint(value, 0, size);

		SingleStringGetter((void*)values, current, &currentValue);

	}

	std::string toString()
	{
		std::string str(currentValue);

		return str;
	}

};

#endif