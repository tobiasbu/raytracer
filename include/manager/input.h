
#ifndef _INPUT_H_
#define _INPUT_H_

#include "manager\appattributes.h"
#include "others\binaryproperty.hpp"

#include <list>
#include <map>

///////////////////////////////////////////////////////////////////////////////////////
/// /brief Class for loading scene files
///
///////////////////////////////////////////////////////////////////////////////////////
class Input {

private:

	// TODO:
	// not enum, instead use string
	enum AssignType {
		_None,
		_Light,
		_Filter,
		_Texture,
		_CubeMap,
		_Material,
		_Sphere,
		_Plane,
		_Mesh
	};


	///////////////////////////////////////////////////////////////////////////////////////
	/// /brief Just hold the AppAtributes pointer to apply values
	///
	///////////////////////////////////////////////////////////////////////////////////////
	AppAttributes * out;

	///////////////////////////////////////////////////////////////////////////////////////
	/// /brief Assignament type when the token start with 'begin' key
	/// Zero means no assignment
	///
	///////////////////////////////////////////////////////////////////////////////////////
	unsigned int assignIndex = 0;
	bool readAssignMode = false;
	unsigned int _textureIndex;

	// prop
	std::vector<std::list<BinaryProperty *>*> _assignamentsProperties;

	// indexes map
	std::map<AssignType, std::vector<unsigned int> *> _assignamentsIndexesMap;

	short readColorFormat = 0;
	std::string camTarget;

	///////////////////////////////////////////////////////////////////////////////////////
	/// /brief This fuction read generic tokens key and apply theirs values
	///
	///////////////////////////////////////////////////////////////////////////////////////
	void readTokens(const std::vector<std::string> & tokens);

	///////////////////////////////////////////////////////////////////////////////////////
	/// /brief This fuction read the 'begin' token header
	///
	/// If the key is 'begin', the assignment read mode is activated. 
	///////////////////////////////////////////////////////////////////////////////////////
	bool readAssignamentScope(const std::vector<std::string> & tokens, unsigned int & index, std::string & elementName);

	///////////////////////////////////////////////////////////////////////////////////////
	/// /brief This fuction read the objects tokens
	/// 
	/// The objects are 3D Primitives, Lights, Filters and so on.
	/// When the key is 'end' finish the assignment.
	/// At end of the 'load' function create the scene objects.
	///////////////////////////////////////////////////////////////////////////////////////
	void readAssignamentTokens(const std::vector<std::string> & tokens);

	void parsePropreties();

	unsigned int createAssignamentList(AssignType type, unsigned int & propIndex);

	// TODO best parser for everthing
	void parseTextures();
	void parseCubeMap();
	void parseMaterials();
	void parseLights();
	void parseMeshes();
	void parseFilters();
	void parsePrimitives();
	

	void parse3DPrimitive(const AssignType type, std::vector<unsigned int> * indexes);
	void parseCamera(const std::vector<std::string> & tokens);
	vec2 parseVector2(const std::vector<std::string> & tokens, int atIndex = 1);
	vec3 parseVector3(const std::vector<std::string> & tokens, int atIndex = 1);
	vec4 parseVector4(const std::vector<std::string> & tokens, int atIndex = 1, bool isColor = true);


public:

	~Input();

	///////////////////////////////////////////////////////////////////////////////////////
	/// /brief Load scene file.
	///
	/// /param filename		The path of file
	/// /param attributes	Application Attributes
	///
	///////////////////////////////////////////////////////////////////////////////////////
	bool load(const char * filename, AppAttributes & attributes);

	void clear();

	int loadExampleScene(const char * exampleSceneName, AppAttributes & attributes);
};

#endif