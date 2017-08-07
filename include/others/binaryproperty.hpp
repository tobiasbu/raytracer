
#ifndef _BINARYPROPERTY_HPP_
#define _BINARYPROPERTY_HPP_

#include <string>

// TODO proprety type traits with #defines
// RTTI and stuff
enum PropertyType {
	Char,
	Float,
	Vector3,
	Vector4,
	String
};


/// This struct hold a void * pointer of object.
/// It's a easy way to make a structure of every kind of object pointer
/// I was testing some pointer stuff and I decide to manipulate pointer. Is a good or bad decision? Not Idea.
struct BinaryProperty {
	const std::string name;
	//unsigned int dataSize;
	void * data;
	PropertyType type;

	BinaryProperty(const std::string & name, void * data, PropertyType type)
		:
		name(name)
	{

		//this->name = name;
		this->data = data;
		this->type = type;

		/*switch (type) {
		default: this->dataSize = 0; break;
		case PropertyType::Vector3: this->dataSize = sizeof(float) * 3; break;
		case PropertyType::Vector4: this->dataSize = sizeof(float) * 4; break;
		case PropertyType::Float: this->dataSize = sizeof(float); break;
		case PropertyType::String: this->dataSize = sizeof((std::string*)&data); break; // static_cast<std::string*>(data)
			//strlen(((char**)&data)[0]);/*sizeof((char**)&data); break;
		}*/

	}

	

	/*~BinaryProperty()
	{
	dataSize = 0;

	if (this->type == PropertyType::String) {
	delete (std::string*)(data);
	}
	else if (this->type == PropertyType::Vector3) {
	delete (vec3*)(data);
	}
	else if (this->type == PropertyType::Float) {
	delete (float*)(data);
	}
	else if (this->type == PropertyType::Vector4) {
	delete (vec4*)(data);
	}

	}*/

	/*const vec3 & getvec3() const {
		return *(vec3*)(data);
	}

	const vec4 & getvec4() const {
		return *(vec4*)(data);
	}*/

	const char * getchararr() const {
		//char *arr_ptr = &a_str[0];
		return ((char**)&data)[0];
	}

	template<typename T>
	const T & get() const
	{
		return *(T*)(data);
	}

	template<typename T >
	T & getref() 
	{
		return *(T*)(&data);
		//(*(T**)&data)[0];
	}

	// TODO overloading functions
	template<typename T >
	static BinaryProperty * create(std::string name, T * value, PropertyType type)
	{
		return new BinaryProperty(name, value, type);
	}

	/*static void get(const BinaryProperty & prop, void * generic)
	{
	if (prop.type == TokenType::Vector3) {
	generic = (vec3*)(prop.data);

	//return (void *)(&prop.getvec3());
	}
	}*/

};

#endif