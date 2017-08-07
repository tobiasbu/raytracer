
#include "others\log.h"
#include "manager\input.h"

#include "entity\sphere.h"
#include "entity\plane.h"
#include "render\meshloader.h"
#include "render\meshfactory.h"
#include "render\mesh.h"
#include "math/vec4.h"


#include "utils/stringutils.h"

#include <cstring>
#include <iostream>
#include <fstream>
#include <random>
#include <sstream>

Input::~Input() 
{
	clear();
}

bool Input::load(const char * filename, AppAttributes & attributes) {

	std::ifstream FILE(filename);

	

	if (!FILE.is_open()) {
		
		return false;
	}

	Debug::log("Reading scene file: %s \n", filename);

	// references setup
	//assign = 0;	
	out = & attributes;

	// variables for reading
	std::string str;
	std::vector<std::string> tokens;

	// create list only for textures
	createAssignamentList(AssignType::_Texture, _textureIndex);

	while (std::getline(FILE, str))
	{
		// it's just starting with comment then continue (fast way)
		if (str[0] == '#' || (str.length() >= 2 && (str[0] == '/' && str[1] == '/'))) {
			continue;
		}
		else if (str.length() > 0){

			// read tokens and set to application attributes
			// remove comments if have any "#" or "//"
			StringUtils::removeAtDelimeter(str, "#");
			StringUtils::removeAtDelimeter(str, "//");

			// tokenize line get key and value
			StringUtils::tokenize<std::vector<std::string>>(str, tokens, ": \t", true);

			// read tokens, at least must have 1 token (key : value, value, value...)
			if (tokens.size() >= 1) {

				
				// read assignaments objects
				if (tokens[0] == "begin") {
					
					std::string name;
					readAssignMode = readAssignamentScope(tokens, assignIndex, name);

					if (readAssignMode) {
						std::string * str = new std::string;
						*str = name;
						_assignamentsProperties[assignIndex]->push_back(new BinaryProperty("elementName", str, PropertyType::String));
					}

					//std::cout << name << std::endl;

				}
				else if (tokens[0] == "end") // end tag
					readAssignMode = false;
				else if (readAssignMode) // read tags for objects
					readAssignamentTokens(tokens);
				else  // otherwise general tags
					readTokens(tokens);
				
			}

			// clear tokens for next line
			tokens.clear();
		}
	}


	
	//BinaryProperty teste(new vec3(0, 1, 0), TokenType::Vector3);
	//BinaryProperty teste2("bom dia", TokenType::String);
	
	//void * type = nullptr;
	//std::cout << *(vec3*)teste.data << " "  << std::endl;
	//BinaryProperty::get(teste, type);
	//std::cout << *(vec3*)(type) << type << std::endl;
	//*(vec3*)teste.data

	
	
	FILE.close();

	parsePropreties();

	clear();

	out = nullptr;

	return true;

}

void Input::readTokens(const std::vector<std::string> & tokens) {

	const std::string & key = tokens[0];
	const int size = tokens.size();

	// tokens that must have at least two values
	if (size > 1) {

		// read program type
		if (key == "program") 
		{

			int value = 0;

			if (tokens[1] == "rc")
				value = 0;
			else if (tokens[1] == "rt")
				value = 1;

			out->options.programType = value;
		}
		else if (key == "trace")
		{
			int value = 0;

			if (tokens[1] == "list")
				value = 0;
			else if (tokens[1] == "aabb")
				value = 1;
			else if (tokens[1] == "bvh")
				value = 2;
			else if (tokens[1] == "sah-bvh")
				value = 3;

			out->options.traceMethod = value;
		}
		else if (key == "projection")
		{

			float fov = StringUtils::parseFloat(tokens[1]);
			out->projection.setPerspective(fov, 0, 1000);

		} 
		else if (key == "resolution" || key == "sceen-size") 
		{
			int w = 640;
			int h = 480;
			
			w = StringUtils::parseInt(tokens[1]);

			if (size >= 3)
				h = StringUtils::parseInt(tokens[2]);
			else
				h = w;
			
			out->projection.setViewSize(w, h);

		}
		else if (key == "output")
		{
			/*std::string fname = StringUtils::fileName();
			std::string ext = StringUtils::fileExtension(tokens[1]);
			if (ext.length() < 3)
				ext = ".png";*/

			out->options.outputfileName = tokens[1];
		}
		else if (key == "camera")
		{
	
			int type = 0;
			int max = size - 1;

			if (max > 9)
				max = 9;

			for (int i = 1; i < max; i += 3) {

				vec3 val = parseVector3(tokens, i);

				if (type == 0)
					out->camera.position = val;
				else if (type == 1)
					out->camera.setDirection(val);
				else if (type == 2)
					out->camera.up = val;

				type++;
				/*std::istringstream tok(tokens[i]);
				
				int j = (i-1) % 3;

				tok >> val[j];

				if (j == 2) {
					if (type == 0)
						out->camera.position = val;
					else if (type == 1)
						out->camera.setDirection(val);
					else if (type == 2)
						out->camera.up = val;

					type++;
				}*/
			}

		}
		else if (key == "background")
		{
			
			vec4 c = parseVector4(tokens, 1);
			out->scene.setBackground(c);
		}
		else if (key == "ambient")
		{
			out->scene.setAmbientColor(parseVector4(tokens, 1));
		}
		else if (key == "shadows")
		{
			out->options.shadowType = StringUtils::parseInt(tokens[1]);
		}
		else if (key == "texture")
		{
			if (out->scene.getTexture(tokens[1]) == nullptr) {
				Texture * tex = new Texture();
				tex->load(tokens[1]);
				out->scene.addTexture(tokens[1], tex);
			}
			
		}
		else if (key == "colorformat")
		{
			if (tokens[1] == "1" || tokens[1] == "normalized" || tokens[1] == "float")
				readColorFormat = 0;
			else if (tokens[1] == "255" || tokens[1] == "integer")
				readColorFormat = 1;
		} 

		else if (key == "anti-alias" || key == "antialias" || key == "anti-aliasing" || key == "antialiasing")
		{
			int val = StringUtils::parseInt(tokens[1]);

			if (val < 0)
				val = 0;

			out->options.antiAliasSamples = val;
		}
		else {

			// parsing camera specific tokens
			size_t pos = key.find("camera-");

			if (pos != std::string::npos) 
			{
				std::string subKey = StringUtils::remove(key, "camera-");

				if (subKey == "pos" || subKey == "eye") {
					out->camera.position = parseVector3(tokens);
				}
				else if (subKey == "dir") {
					out->camera.setDirection(parseVector3(tokens));
				}
				else if (subKey == "up") {
					out->camera.up = parseVector3(tokens);
				}
				else if (subKey == "target") {
					camTarget = tokens[1];
				}
			}
				
			
		}
	}

}

bool Input::readAssignamentScope(const std::vector<std::string> & tokens, unsigned int & index, std::string & elementName)
{
	std::stringstream assignName;
	const std::string & name = tokens[1];

	// valid objects, only textures can not be here!
	if (name == "material" || name == "sphere" || name == "plane" || name == "filter" || name == "light" || name == "mesh" || name == "cubemap") {

		AssignType type = _None;

		
		if (name[0] == 'm' && name[1] == 'a')
			type = AssignType::_Material;
		else if (name[0] == 'm' && name[1] == 'e')
			type = AssignType::_Mesh;
		else if (name[0] == 's')
			type = AssignType::_Sphere;
		else if (name[0] == 'p')
			type = AssignType::_Plane;
		else if (name[0] == 'l')
			type = AssignType::_Light;
		else if (name[0] == 'c')
			type = AssignType::_CubeMap;
		else if (name[0] == 'f')
			type = AssignType::_Filter;

		//std::vector<unsigned int> *  indexes;

		unsigned int elementIndex = createAssignamentList(type, index);
		
		if (tokens.size() >= 3)
			assignName << tokens[2];
		else
			assignName << name << elementIndex;
		

		elementName = assignName.str();


		//indexes->size();

		/*std::map<AssignType, std::vector<unsigned int>*>::iterator it = _assignamentsIndexesMap.find(type);
		std::vector<unsigned int> * indexes;

		// create new element on map
		if (it == _assignamentsIndexesMap.end()) {
			indexes = new std::vector<unsigned int>();
			_assignamentsIndexesMap.insert(std::make_pair(type, indexes));
		}
		else
		{ // get element on map
			indexes = it->second;

		}

		


		// create new list of props
		_assignamentsProperties.push_back(new std::list<BinaryProperty *>);

		// add to indexes map
		indexes->push_back(_assignamentsProperties.size() - 1);		

		// set current index propertie
		index = _assignamentsProperties.size() - 1;*/

		//_assignaments.push_back(std::make_pair(type, new std::list<BinaryProperty*>));
		//if (_assignamentsMap.find(type) == _assignamentsMap.end())
			//_assignamentsMap.insert(std::make_pair(type, new std::list<std::vector<BinaryProperty *>*>));
		
		//_assignamentsNames.insert(std::make_pair(assignName.str(),index));
		//_assignamentsNames.insert(std::make_pair(assignName.str(), std::make_pair(index, type)));



		return true;
	}
	else
		index = -1;

		return false;

}

void Input::readAssignamentTokens(const std::vector<std::string> & tokens)
{
	std::string  key = tokens[0];
	const int size = tokens.size();

	if (size > 1) {
		if (key == "position" || key == "normal" || key == "center" || key == "direction" || key == "scale" || key == "angle" || key == "ks" || key == "kd")
		{
			_assignamentsProperties[assignIndex]->push_back(new BinaryProperty(key, new vec3(parseVector3(tokens)), PropertyType::Vector3));
			//_assignaments[assignIndex].second->push_back(new BinaryProperty(key, new vec3(parseVector3(tokens)), PropertyType::Vector3));
		}
		else if (key == "radius" || key == "offset" || key == "fallout" || key == "intensity" || key == "ns" || key == "ni" || key == "ior" || key == "refraction" || key == "d")
		{
			float val = StringUtils::parseFloat(tokens[1]);

			if (key == "fallout")
				key = "intensity";

			_assignamentsProperties[assignIndex]->push_back(new BinaryProperty(key, new float(val), PropertyType::Float));
	
		}
		else if (key == "fcolor" || key == "color" || key == "icolor") {

			short old = readColorFormat;
			
			if (key[0] == 'f') 
				readColorFormat = 0;
			else if (key[0] == 'i')
				readColorFormat = 1;

			vec4 v = parseVector4(tokens,1,true);
			readColorFormat = old;

			_assignamentsProperties[assignIndex]->push_back(new BinaryProperty("color", new vec4(v), PropertyType::Vector4));
		}
		else if (key == "texture" || key == "bumpmap" || key == "normalmap" || key == "transparentmap") 
		{
			
			std::string * str = new std::string(tokens[1]);

			BinaryProperty * prop = new BinaryProperty(key, str, PropertyType::String);
			_assignamentsProperties[assignIndex]->push_back(prop);
			// add to textures list too
			BinaryProperty * prop2 = new BinaryProperty(key, new std::string(tokens[1]), PropertyType::String);
			_assignamentsProperties[_textureIndex]->push_back(prop2);
		}
		else if (key == "material" || key == "type" || key == "file" || key == "path" || key == "suffix" || key == "format" || key == "extension")
		{
			BinaryProperty * prop = new BinaryProperty(key, new std::string(tokens[1]), PropertyType::String);
			_assignamentsProperties[assignIndex]->push_back(prop);
		}
		else if (key == "pattern") {

			std::stringstream str;
			for (size_t i = 1; i < tokens.size(); i++)
				str << tokens[i] << " ";

			_assignamentsProperties[assignIndex]->push_back(new BinaryProperty(key, new std::string(str.str()), PropertyType::String));
		}
		else if (key == "contrast" || key == "brightness" || key == "grayscale" || key == "posterize") {
			float val = StringUtils::parseFloat(tokens[1]);
			_assignamentsProperties[assignIndex]->push_back(new BinaryProperty(key, new float(val), PropertyType::Float));
		}
		else if (key == "sharpen" || key == "blur" || key == "unsharpmask" || key == "emboss" || key == "gaussianblur" || key == "sobel" || key == "edge" || key == "edgedetection") {
			float val = StringUtils::parseFloat(tokens[1]);
			_assignamentsProperties[assignIndex]->push_back(new BinaryProperty(key, new std::string(key), PropertyType::String));
		}


	}

}

unsigned int Input::createAssignamentList(AssignType type, unsigned int & propIndex)
{
	std::map<AssignType, std::vector<unsigned int>*>::iterator it = _assignamentsIndexesMap.find(type);
	std::vector<unsigned int> * indexes;

	// create new element on map
	if (it == _assignamentsIndexesMap.end()) {
		indexes = new std::vector<unsigned int>();
		_assignamentsIndexesMap.insert(std::make_pair(type, indexes));
	}
	else
	{ // get element on map
		indexes = it->second;

	}
	// create new list of props
	_assignamentsProperties.push_back(new std::list<BinaryProperty *>);

	// add to indexes map
	indexes->push_back(_assignamentsProperties.size() - 1);

	//  set the current index of properties list
	propIndex = _assignamentsProperties.size() - 1;

	// return the element index;
	return indexes->size() - 1;
	
}

void Input::parsePropreties()
{
	std::map<AssignType, std::vector<unsigned int>*>::iterator it;

	// important: first textures
	// material can assign textures, so for safety (null pointer cases) we create textures first
	parseTextures();

	// parse cubemap
	parseCubeMap();
	
	// then parse materials
	// same case of materials but for objects that can have a material
	parseMaterials();

	// parse lights
	parseLights();

	// parse primitives and meshes
	parsePrimitives();

	// parse filters
	parseFilters();

	// parse meshes
	//parseMeshes();

	// parse filters

	// create spheres, planes, meshes, filters

	
	//std::list<BinaryProperty *>::iterator it_list;

	/*it = _assignamentsIndexesMap.find(AssignType::_Sphere);

	if (it != _assignamentsIndexesMap.end())
	{ // scene has spheres

		Primitive * obj = nullptr;

		for (size_t i = 0; i < it->second->size(); i++)
		{
			unsigned int propsListIndex = (*it->second)[i];

			std::list<BinaryProperty *> * list = _assignamentsProperties[propsListIndex];

			if (list->size() > 0) {

				obj = new Sphere();

				for (std::list<BinaryProperty *>::iterator it_list = list->begin(); it_list != list->end(); it_list++)
				{
					const BinaryProperty * __restrict prop = (*it_list);

					if (prop->name == "radius")
						static_cast<Sphere*>(obj)->setRadius(prop->get<float>());
					else if (prop->name == "position" || prop->name == "center")
						obj->position = prop->get<vec3>();
					else if (prop->name == "material") {
						//std::cout << prop->get<std::string>() << std::endl;
						obj->setMaterial(out->scene.getMaterial(prop->get<std::string>()));
					}


					// delete prop
					//delete (*it_list);
				}

				// clear list
				//list->clear();
				

				if (obj != nullptr)
					out->scene.addPrimitive(obj);

				obj = nullptr;
			}

			// delete list
			//delete list;

		}*/

		
		//it->second->clear();
		//delete it->second;

	//}

	//Primitive * object = nullptr;

	/*for (std::vector< std::pair<AssignType, std::list<BinaryProperty *> *>>::iterator it = _assignaments.begin(); it != _assignaments.end(); ++it)
	{

		AssignType type = it->first;
		if (type == AssignType::_Sphere) {
			object = new Sphere();

			for (std::list<BinaryProperty *>::iterator it_list = (*it).second->begin(); it_list != (*it).second->end(); ++it_list)
			{

				const BinaryProperty * __restrict prop = (*it_list);


				if (prop->name == "radius")
					static_cast<Sphere*>(object)->setRadius(prop->get<float>());
				else if (prop->name == "position" || prop->name == "center")
					static_cast<Sphere*>(object)->position = prop->get<vec3>();
			}

			if (object != nullptr)
			{
				out->scene.addPrimitive(object);
				object = nullptr;
			}



		}
		
		
	}*/
}

void Input::parseTextures()
{
	std::list<BinaryProperty *> * list = _assignamentsProperties[_textureIndex];

	for (std::list<BinaryProperty *>::iterator it = list->begin(); it != list->end(); it++)
	{
		BinaryProperty * prop = (*it);
		//char * n = prop->getptr<char*>();
		Texture * texture = out->scene.getTexture(prop->get<std::string>());
		if (texture == nullptr) 
		{
			std::string name = prop->get<std::string>();
			texture = new Texture();
			if (!texture->load(name))
				Debug::log("Texture: Failed to load file: %s", name);
			
			out->scene.addTexture(name, texture);
		}
	}
}

void Input::parseCubeMap()
{
	std::map<AssignType, std::vector<unsigned int>*>::iterator it = _assignamentsIndexesMap.find(AssignType::_CubeMap);

	if (it != _assignamentsIndexesMap.end())
	{

		// check if a cubemap was loaded, otherwise we just ignore
		if (out->scene.getCubeMap() == nullptr) {

			for (size_t i = 0; i < it->second->size(); i++)
			{
				unsigned int propsListIndex = (*it->second)[i];

				std::list<BinaryProperty *> * list = _assignamentsProperties[propsListIndex];

				if (list->size() > 0) {

					std::string path = "";
					std::string fsuffix = "";
					std::string format = "";

					for (std::list<BinaryProperty *>::iterator it_list = list->begin(); it_list != list->end(); it_list++)
					{

						const BinaryProperty * __restrict prop = (*it_list);

						if (prop->name == "path")
							path = prop->get<std::string>();
						else if (prop->name == "suffix")
							fsuffix = prop->get<std::string>();
						else if (prop->name == "format")
							format = prop->get<std::string>();

					}

					// load cubemap
					if (path.length() > 0 && fsuffix.length() > 0 && format.length() > 0)
					{
						CubeMap * cubemap = new CubeMap();

						format = StringUtils::toLower(format);

						std::stringstream str("");
						std::string suffix[6] = { "left", "right", "top", "bottom", "front", "back" };

						for (int i = 0; i < 6; i++)
						{
							str << path << fsuffix << suffix[i] << "." << format;

							// check first if we have loaded this texture before
							Texture * texture = out->scene.getTexture(str.str());

							// nopes, then load it
							if (texture == nullptr)
							{
								texture = new Texture();

								std::string fname = str.str();

								if (!texture->load(fname)) {
									Debug::log("CubeMap Texture: Failed to load file: %s\n", fname.c_str());
									delete texture;
									texture = nullptr;
								} 
								
							}

							// if we sucefully loaded, add to cubemap
							if (texture != nullptr)
								cubemap->setTexture((CubeMap::CubeMapFace)i, texture);

							str.str("");
						}

						out->scene.setCubeMap(cubemap);

						// don't load others cubemap, only one per scene
						break;
					}

				}
			}
		}
	}

}

void Input::parseMaterials() 
{

	std::map<AssignType, std::vector<unsigned int>*>::iterator it = _assignamentsIndexesMap.find(AssignType::_Material);

	if (it != _assignamentsIndexesMap.end())
	{
		
		for (size_t i = 0; i < it->second->size(); i++)
		{
			unsigned int propsListIndex = (*it->second)[i];

			std::list<BinaryProperty *> * list = _assignamentsProperties[propsListIndex];

			if (list->size() > 0) {

				Material * material = new Material;
				std::string name = "___default-material_";

				for (std::list<BinaryProperty *>::iterator it_list = list->begin(); it_list != list->end(); it_list++)
				{
					
					const BinaryProperty * __restrict prop = (*it_list);

					if (prop->name == "texture")
						material->setTexture(out->scene.getTexture(prop->get<std::string>()));
					else if (prop->name == "bumpmap" || prop->name == "normalmap")
						material->setNormalMap(out->scene.getTexture(prop->get<std::string>()));
					else if (prop->name == "transparentmap")
						material->setTransparentMap(out->scene.getTexture(prop->get<std::string>()));
					else if (prop->name == "name" || prop->name == "elementName")
						name = prop->get<std::string>();
					else if (prop->name == "kd")
						material->setProperty(MAT_DIFFUSE, prop->get<vec3>());
					else if (prop->name == "ks")
						material->setProperty(MAT_SPECULAR, prop->get<vec3>());
					else if (prop->name == "ns")
						material->setProperty(MAT_SHININESS, prop->get<float>());
					else if (prop->name == "ni")
						material->setProperty(MAT_REFRACTION, prop->get<float>());
					else if (prop->name == "d")
						material->setProperty(MAT_TRANSPARENCY, prop->get<float>());
					else if (prop->name == "pattern") 
					{
						std::string pat = prop->get<std::string>();
						std::vector<std::string> s = StringUtils::split(pat, ' ', true);

						if (s.size() > 1) 
						{
							Pattern * pattern = new Pattern;

							if (s[0] == "checkboard")
								pattern->kind = 0;
							else if (s[0] == "stripe")
								pattern->kind = 1;

							size_t size = s.size();

							for (size_t j = 1; j < size; j++) {

								if (j + 1 < size) {
									if (s[j] == "-s")
									{
										vec2 sc = parseVector2(s, j + 1);
										if (sc <= 0)
											sc = 0.1f;
										pattern->scale = sc;
										j++;
									}
									else if (s[j] == "-ca")
									{
										vec3 sc = parseVector3(s, j + 1);
										pattern->a = Color(sc, 1);
										j++;
									}
									else if (s[j] == "-cb")
									{
										vec3 sc = parseVector3(s, j + 1);
										pattern->b = Color(sc, 1);
										j++;
									}
									else if (s[j] == "-a")
									{
										float a = StringUtils::parseFloat(s[j + 1]);
										pattern->angle = a;
										j++;
									}
								}
							}

							material->setPattern(pattern);
						}

					}
				}

				out->scene.addMaterial(name, material);

			}

		}
	}
}

void Input::parseLights()
{
	std::map<AssignType, std::vector<unsigned int>*>::iterator it = _assignamentsIndexesMap.find(AssignType::_Light);

	if (it != _assignamentsIndexesMap.end())
	{

		for (size_t i = 0; i < it->second->size(); i++)
		{
			unsigned int propsListIndex = (*it->second)[i];

			std::list<BinaryProperty *> * list = _assignamentsProperties[propsListIndex];

			if (list->size() > 0) {

				//Material * material = new Material;
				std::string type;
				Color col;
				vec3 pos(0, -1, 0);
				vec3 dir(0, 0, 1);
				float i = 1;


				for (std::list<BinaryProperty *>::iterator it_list = list->begin(); it_list != list->end(); it_list++)
				{

					const BinaryProperty * __restrict prop = (*it_list);

					if (prop->name == "color")
						col = prop->get<vec4>();
					else if (prop->name == "position" || prop->name == "pos")
						pos = prop->get<vec3>();
					else if (prop->name == "intensity")
						i = prop->get<float>();
					else if (prop->name == "direction")
						dir = prop->get<vec3>();
					else if (prop->name == "type")
						type = prop->get<std::string>();
					
				}


				Light * l;

				if (type == "directional") {
					l = new DirectionalLight();
					static_cast<DirectionalLight*>(l)->setDirection(dir);// = dir;
				}
				else
					l = new PointLight();
				
				l->color = col;
				l->position = pos;
				l->intensity = i;
				

				out->scene.addLight(l);

			}

		}
	}
}

void Input::parsePrimitives() 
{
	std::vector<Primitive *> objects;
	std::vector<std::string> names;
	int index = 0;
	int endIndexMesh = 0;


	for (std::map<AssignType, std::vector<unsigned int>*>::iterator it = _assignamentsIndexesMap.begin(); it != _assignamentsIndexesMap.end(); ++it) 
	{
		const AssignType & type = it->first;


		if (type == AssignType::_Mesh || type == AssignType::_Sphere || type == AssignType::_Plane)
		{

			for (size_t i = 0; i < it->second->size(); i++)
			{
				unsigned int propsListIndex = (*it->second)[i];

				std::list<BinaryProperty *> * list = _assignamentsProperties[propsListIndex];

				vec3 position;
				vec3 angle;
				vec3 scale = vec3(1, 1, 1);
				std::string name = "__no_name";
				Material * mat = nullptr;

				if (list->size() > 0) 
				{
					if (type == _Sphere)
						objects.push_back(new Sphere);
					else if (type == _Plane)
						objects.push_back(new Plane);

					
					index = objects.size() - 1;
					bool loaded = false;

					for (std::list<BinaryProperty *>::iterator it_list = list->begin(); it_list != list->end(); it_list++)
					{
						const BinaryProperty * prop = (*it_list);

						if (type == _Sphere)
						{
							if (prop->name == "radius")
								static_cast<Sphere*>(objects[index])->setRadius(prop->get<float>());
						}
						else if (type == _Plane)
						{
							if (prop->name == "normal")
								static_cast<Plane*>(objects[index])->setNormal(prop->get<vec3>());
							else if (prop->name == "offset")
								static_cast<Plane*>(objects[index])->setOffset(prop->get<float>());
						}
						else if (type == _Mesh)
						{
							if (prop->name == "file") {

								std::vector<Mesh *> meshes;

								std::string filename = prop->get<std::string>();
								if (!loaded) {
									loaded = MeshLoader::getInstance().loadMesh(filename, meshes,out->options.traceMethod);
									if (!loaded)
										Debug::log("MeshLoader: Failed to load file: %s\n", filename);
									else
									{
										index = objects.size();

										for (size_t i = 0; i < meshes.size(); i++) {
											if (meshes[i] != nullptr)
												objects.push_back(meshes[i]);
										}
										endIndexMesh = objects.size();
									}
								}
							}
						}

						// generic
						if (prop->name == "position" || prop->name == "center")
							position = prop->get<vec3>();
						else if (prop->name == "angle")
							angle = prop->get<vec3>();
						else if (prop->name == "scale")
							scale = prop->get<vec3>();
						else  if (prop->name == "material") {
							//std::cout << prop->get<std::string>() << std::endl;
							mat = out->scene.getMaterial(prop->get<std::string>());
						}
						else if (prop->name == "name" || prop->name == "elementName") {
							name = prop->get<std::string>();
						
						}

					}

					if (type == _Mesh)
					{
						if (loaded) 
						{
							for (int j = index; j < endIndexMesh; j++) {
								objects[j]->position = position;
								objects[j]->angle = angle;
								objects[j]->scale = scale;
								if (mat != nullptr)
									objects[j]->setMaterial(mat);
								names.push_back(name);
							}
						}
					}
					else {
						objects[index]->position = position;
						objects[index]->angle = angle;
						objects[index]->scale = scale;
						objects[index]->setMaterial(mat);
						//if (name != "__no_name")
						names.push_back(name); //= name;
					}
				}

			}
		}
	}

	if (objects.size() > 0) 
	{
		for (size_t i = 0; i < objects.size(); i++) {
			
			out->scene.addPrimitive(objects[i]);

			if (names[i] == camTarget) {
				out->camera.setDirection(objects[i]->position);
				//out->camera.targetObject = i;
			}
		}
	}

}

void Input::parseMeshes()
{
	std::map<AssignType, std::vector<unsigned int>*>::iterator it = _assignamentsIndexesMap.find(AssignType::_Mesh);

	if (it != _assignamentsIndexesMap.end())
	{

		for (size_t i = 0; i < it->second->size(); i++)
		{
			unsigned int propsListIndex = (*it->second)[i];

			std::list<BinaryProperty *> * list = _assignamentsProperties[propsListIndex];

			if (list->size() > 0) {

				std::vector<Mesh *> meshes; // = new Mesh;
				bool loaded = false;
		
				for (std::list<BinaryProperty *>::iterator it_list = list->begin(); it_list != list->end(); it_list++)
				{
					const BinaryProperty * __restrict prop = (*it_list);

					if (prop->name == "file") {

						std::string filename = prop->get<std::string>();



						if (!loaded) {
							loaded = MeshLoader::getInstance().loadMesh(filename, meshes);
							if (!loaded) 
								Debug::log("MeshLoader: Failed to load file: %s\n", filename);
						}
					}
					else if (loaded) {
						if (prop->name == "material")
						{
							//mesh->setMaterial(out->scene.getMaterial(prop->get<std::string>()));
						}
					}


				}

				if (meshes.size() > 0) {
					if (loaded) {
						for (size_t i = 0; i < meshes.size(); i++) {
							if (meshes[i] != nullptr)
								out->scene.addPrimitive(meshes[i]);
						}
					}
				}
				
			}

		}
	}
}

void Input::parseFilters()
{
	std::map<AssignType, std::vector<unsigned int>*>::iterator it = _assignamentsIndexesMap.find(AssignType::_Filter);

	if (it != _assignamentsIndexesMap.end())
	{

		for (size_t i = 0; i < it->second->size(); i++)
		{
			unsigned int propsListIndex = (*it->second)[i];

			std::list<BinaryProperty *> * list = _assignamentsProperties[propsListIndex];

			float contrast = -999;
			float bright = -666;

			if (list->size() > 0) {

				for (std::list<BinaryProperty *>::iterator it_list = list->begin(); it_list != list->end(); it_list++)
				{

					const BinaryProperty * __restrict prop = (*it_list);

					const std::string & name = prop->name;

					if (name == "sharpen" || name == "blur" || name == "edge" || name == "gaussianblur" || name == "unsharpmask" || name == "emboss" || name == "edgedetection" || name == "sobel")
						out->filters.addFilter(prop->name);
					else if (prop->name == "grayscale" || prop->name == "posterize")
						out->filters.addColor(prop->name, prop->get<float>(), 0);
					else if (prop->name == "contrast")
						contrast = prop->get<float>();
					else if (prop->name == "brightness")
						bright = prop->get<float>();
					
				}

				if (contrast != -999 && bright != -666)
				out->filters.addColor("cb", contrast, bright);

				
			}

		}
	}
}

vec4 Input::parseVector4(const std::vector<std::string> & tokens, int atIndex, bool isColor)
{
	vec4 v(0,0,0,1);
	v.w = 1;

	int max = tokens.size() - atIndex;

	if (max > 4)
		max = 4;

	for (int i = atIndex; i < atIndex + max; i++) {
		std::istringstream tok(tokens[i]);

		int j = (i - atIndex) % 4;

		if (max == 1) {
			// scalar
			float val;
			tok >> val;
				v.set(val);
				break;
		}
		else
			tok >> v[j];
	}


	if (isColor && readColorFormat == 1) {
		v.x /= 255.0; v.y /= 255.0; v.z /= 255.0; v.w /= 255.0;
	}

	return v;
}

vec3 Input::parseVector3(const std::vector<std::string> & tokens, int atIndex)
{

	vec3 tmp(0.0);

	int max = tokens.size() - atIndex;

	if (max > 3)
		max = 3;

	for (int i = atIndex; i < atIndex+max; i++) {
		std::istringstream tok(tokens[i]);

		int j = (i - atIndex) % 3;

		if (max == 1)  {// scalar
			float val;
			tok >> val;
			tmp.set(val);
			break;
		}
		else
			tok >> tmp[j];
	}

	//std::cout << tmp << std::endl;

	return tmp;

}

vec2 Input::parseVector2(const std::vector<std::string> & tokens, int atIndex) 
{
	vec3 tmp(0.0);

	int max = tokens.size() - atIndex;

	if (max > 2)
		max = 2;

	for (int i = atIndex; i < atIndex + max; i++) {
		std::istringstream tok(tokens[i]);

		int j = (i - atIndex) % 3;

		if (max == 1)  {// scalar
			float val;
			tok >> val;
			tmp.set(val);
			break;
		}
		else
			tok >> tmp[j];
	}

	

	return tmp;
}

int Input::loadExampleScene(const char * exampleSceneName, AppAttributes & attributes) {

	out = &attributes;

	//Debug::log("Load Raytracer scene example\n");


	//int w = 800, h = 600;

	// setup program type raytracer or raycast example
	//if (strcmp(exampleSceneName, "raycast") != 0 || strcmp(exampleSceneName,"ray") != 0)
		out->options.programType = 0;
	//else
	//	out->programType = 1;

	// setup camera
	out->camera.position = vec3(0, 0 ,-5);
	out->camera.up = vec3(0, -1, 0);
	out->camera.setDirection(vec3(0, 0, 1));
	//out->camera.up = vec3(0, 1, 0);

	// setup projection
	//out->projection.setViewSize(out->options., h);
	out->projection.setCamera(&out->camera);
	out->projection.setPerspective(60, 0.03f, 1000);

	int r = 1;
	
	if (strcmp(exampleSceneName, "test0") == 0) {

		out->options.renderMethod = 0;
		out->options.shadowType = 2;

		// setup objects
		Sphere * sphere_r = new Sphere(0.85f, vec3(3, -2.5f, 0));
		Sphere * sphere_r0 = new Sphere(1, vec3(1.5f, 0, 0));
		Sphere * sphere_r1 = new Sphere(0.5f, vec3(3, 2.5f, 0));
		Sphere * sphere2 = new Sphere(1.25f, vec3(-2, -1.75f, 0));
		Sphere * sphere3 = new Sphere(1, vec3(-2, 1.5f, 1));

		DirectionalLight * dirl = new DirectionalLight(vec3(0, 0, -5), vec3(0, 0, 0));
		dirl->setDirection(vec3(0, 0, -1));
		dirl->intensity = 1;
		out->scene.addLight(dirl);

		out->scene.addPrimitive(sphere_r);
		out->scene.addPrimitive(sphere_r0);
		out->scene.addPrimitive(sphere_r1);
		out->scene.addPrimitive(sphere2);
		out->scene.addPrimitive(sphere3);

	}
	else if (strcmp(exampleSceneName, "test1") == 0) {

		out->options.renderMethod = 0;
		out->options.shadowType = 1;
		out->options.maxDepth = 6;

		Material * red = new Material;
		red->setProperty(MAT_DIFFUSE, vec3(1, 0.15f, 0));
		red->setProperty(MAT_REFRACTION, 0.085);
		red->setProperty(MAT_TRANSPARENCY, 0.5);
		red->setProperty(MAT_TRANSMITTANCE, vec3(0.9f));

		Material * green = new Material;
		green->setProperty(MAT_DIFFUSE, vec3(0.1, 1, 0.2f));
		green->setProperty(MAT_REFRACTION, 0.2f);
		green->setProperty(MAT_TRANSPARENCY, 0.001f);
		
		Material * blue = new Material;
		blue->setProperty(MAT_DIFFUSE, vec3(0.3f, 0.2f, 1));
		blue->setProperty(MAT_REFRACTION, 0.2f);
		blue->setProperty(MAT_TRANSPARENCY, 0.001f);
		Material * y = new Material;
		y->setProperty(MAT_DIFFUSE, vec3(1, 0.85f, 0.05f));
		y->setProperty(MAT_REFRACTION, 0.2f);
		y->setProperty(MAT_TRANSPARENCY, 0.001f);
		Material * p = new Material;
		p->setProperty(MAT_DIFFUSE, vec3(0.4f, 1, 1));
		p->setProperty(MAT_REFRACTION, 0.2f);
		p->setProperty(MAT_TRANSPARENCY, 0.001f);
		out->scene.addMaterial("blue", blue);
		out->scene.addMaterial("green", green);
		out->scene.addMaterial("red", red);

		// setup objects
		Sphere * sphere_r = new Sphere(0.85, vec3(3, -2.5, 0));
		sphere_r->setMaterial(red);
		Sphere * sphere_r0 = new Sphere(1, vec3(1.5, 0, 0));
		sphere_r0->setMaterial(green);
		Sphere * sphere_r1 = new Sphere(0.5, vec3(3, 2.5, 0));
		sphere_r1->setMaterial(blue);
		Sphere * sphere2 = new Sphere(1.25, vec3(-2, -1.75, 0));
		sphere2->setMaterial(p);
		Sphere * sphere3 = new Sphere(1, vec3(-2, 1.5, 1));
		sphere3->setMaterial(y);

		DirectionalLight * dirl = new DirectionalLight(vec3(0, 0, -5), vec3(0, 0, 0));
		dirl->setDirection(vec3(0, 0, -1));
		dirl->intensity = 1;
		out->scene.addLight(dirl);

		out->scene.addPrimitive(sphere_r);
		out->scene.addPrimitive(sphere_r0);
		out->scene.addPrimitive(sphere_r1);
		out->scene.addPrimitive(sphere2);
		out->scene.addPrimitive(sphere3);

	}
	else if (strcmp(exampleSceneName, "test2") == 0)
	{
		

		srand(4533);

		out->options.renderMethod = 0;
		out->options.shadowType = 1;

		DirectionalLight * dirl = new DirectionalLight(vec3(0, 0, -5), vec3(0, 0, 0));
		dirl->setDirection(vec3(0, 0, -1));
		dirl->intensity = 1;

		Material * red = new Material;
		red->setProperty(MAT_DIFFUSE, vec3(1, 0, 0));

		Material * green = new Material;
		green->setProperty(MAT_DIFFUSE, vec3(0, 1, 0));
		Material * blue = new Material;
		blue->setProperty(MAT_DIFFUSE, vec3(0, 0, 1));
		out->scene.addMaterial("blue", blue);
		out->scene.addMaterial("green", green);
		out->scene.addMaterial("red", red);
		out->scene.addLight(dirl);

		for (int i = 0; i < 1001; i++)
		{
			float x = Math::randomRange(-20, 20);
			float y = Math::randomRange(-20, 20);
			float z = Math::randomRange(6, 100);
			float r = Math::randomRange(0.5, 1.5);
			Sphere * sphere = new Sphere(r, vec3(x, y, z));

			int rd = rand() % 100;

			if (rd >= 0 && rd < 33)
				sphere->setMaterial(red);
			else if (rd >= 33 && rd < 66)
				sphere->setMaterial(green);
			else if (rd >= 66 && rd < 99)
				sphere->setMaterial(blue);
			out->scene.addPrimitive(sphere);
		}
	}
	else if (strcmp(exampleSceneName, "test3") == 0)
	{


		srand(81234);

		out->options.renderMethod = 0;
		//out->options.shadowType = 1;

		DirectionalLight * dirl = new DirectionalLight(vec3(0, 0, -5), vec3(0, 0, 0));
		dirl->setDirection(vec3(0, 0, -1));
		dirl->intensity = 0.75;

		Material * red = new Material;
		red->setProperty(MAT_DIFFUSE, vec3(1, 0, 0));
		red->setProperty(MAT_REFRACTION, 0.085);
		red->setProperty(MAT_TRANSPARENCY, 0.5);

		Material * green = new Material;
		green->setProperty(MAT_DIFFUSE, vec3(0, 1, 0));
		green->setProperty(MAT_REFRACTION, 0.2);
		green->setProperty(MAT_TRANSPARENCY, 0.8);


		Material * blue = new Material;
		blue->setProperty(MAT_DIFFUSE, vec3(0, 0, 1));
		blue->setProperty(MAT_REFRACTION, 0.2);
		blue->setProperty(MAT_TRANSPARENCY, 0.8);
		//red->setProperty(MAT_TRANSMITTANCE, vec3(0.1, 0.1, 0.1));

		out->scene.addMaterial("blue", blue);
		out->scene.addMaterial("green", green);
		out->scene.addMaterial("red", red);
		out->scene.addLight(dirl);

		for (int i = 0; i < 1001; i++)
		{
			float x = Math::randomRange(-20, 20);
			float y = Math::randomRange(-20, 20);
			float z = Math::randomRange(6, 100);
			float r = Math::randomRange(0.5, 1.5);
			Sphere * sphere = new Sphere(r, vec3(x, y, z));

			int rd = rand() % 100;

			if (rd >= 0 && rd < 33)
				sphere->setMaterial(red);
			else if (rd >= 33 && rd < 66)
				sphere->setMaterial(green);
			else if (rd >= 66 && rd < 99)
				sphere->setMaterial(blue);
			out->scene.addPrimitive(sphere);
		}
	}
	else if (strcmp(exampleSceneName, "test4") == 0)
	{
		out->options.traceMethod = 1;
		Plane * plane = new Plane();
		Sphere * sphere = new Sphere(1, vec3(0, -0.25, 0));
		Sphere * sphere2 = new Sphere(0.5, vec3(1.2, -0.25, 0));
		Sphere * sphere3 = new Sphere(1, vec3(-2, -0.5, 0));
		PointLight * l = new PointLight;
		l->position = vec3(0,-1.5,-2.5);
		l->intensity = 20;
		out->scene.addLight(l);
		out->scene.addPrimitive(plane);
		out->scene.addPrimitive(sphere3);
		out->scene.addPrimitive(sphere2);
		out->scene.addPrimitive(sphere);
	}
	else
	{
		r = -2;
	}

	return r;
	 

	// y negative is up

	//Plane * plane = new Plane();
	//Mesh * q = MeshFactory::quad();
	//Mesh * q2 = MeshFactory::quad();
	//
	
	//
	//mat->setUnlit(true);
	//sphere->setMaterial(mat);
	//sphere2->setMaterial(mat);
	//Plane * plane = new Plane(vec3(0,1,0),0);

	//Material * mat = new Material(vec3(0.8f, 0.15f, 0.4f));
	//Material * mat2 = new Material(vec3(1, 1, 0));

//	mat2->_reflective = 0;
//	mat2->_refraction = 0;
//	mat2->_d = 1;
	//mat2->_specularColor = vec3::zero;

	//sphere->setMaterial(mat2);

	
	//mat2->_n = 25;
	//q->setMaterial(mat2);
	//q->position = vec3(0, -0.5, 0);
	//q2->position = vec3(0, -10, 0);


	//mat->_color = vec3(0.2, 0.2, 0.2);
	//mat->setUnlit(false);
	//plane->setMaterial(mat);

	
	//out->scene.addPrimitive(q);
	//out->scene.addPrimitive(q2);
	//
	

	//out->scene.addLight(new DirectionalLight(vec3(0,-1,-2),vec3(0,0,0),Color::white, 0.5) );
	//out->scene.addLight(new PointLight(vec3(-2, -4, 0), Color::white, 100));
	//out->scene.addLight(new PointLight(vec3(-1, -1, -3), Color::yellow, 40));

	/*std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(-1, 1);

	int numSpheres = 32;
	gen.seed(0);
	for (int i = 0; i < numSpheres; ++i) {


		vec3 randPos((0.5 - dis(gen)) * 10, (0.5 - dis(gen)) * 10, (0.5 + dis(gen) * 10));
		std::cout << randPos << std::endl;
		float randRadius = (0.5 + dis(gen) * 0.5);
		//objects.push_back(std::unique_ptr<Primitive>(new Sphere(randPos, randRadius)));
		out->scene.addPrimitive(new Sphere(randRadius, randPos));
	}*/


	out = nullptr;

}


void Input::clear()
{
	for (std::vector< std::list<BinaryProperty *> * > ::iterator it = _assignamentsProperties.begin(); it != _assignamentsProperties.end(); ++it) {

		std::list<BinaryProperty *> * list = (*it);

		while (!(*it)->empty()) {
			//BinaryProperty * (*it) = (*it)->front();
			delete (*it)->front()->data;
			(*it)->front()->data = nullptr;
			if ((*it)->front() != nullptr /*&& (*it)->data != nullptr*/)
				delete (*it)->front();
			
			//= nullptr;
			(*it)->front() = nullptr;
			list->pop_front();
		}



		/*for (std::list<BinaryProperty *>::iterator it_list = list->begin(); it_list != list->end(); ++it_list)
		{
			if ((*it_list) != nullptr && (*it_list)->data != nullptr) {
				
		
				std::cout << (*it_list)->name << " ";
				if ((*it_list)->type == PropertyType::String) {

					std::string * str = (static_cast<std::string*>((*it_list)->data));
					std::cout << *str << std::endl;
					//str->clear();
					//delete str;
				} 

				delete (*it_list);
				
			}
			
				(*it_list)->data = nullptr;
				(*it_list) = nullptr;
			
			
		}*/

		//list->clear();
		delete (*it);
		(*it) = nullptr;
	}

	_assignamentsProperties.clear();

	for (std::map<AssignType, std::vector<unsigned int>*>::iterator it = _assignamentsIndexesMap.begin(); it != _assignamentsIndexesMap.end(); ++it) {

		it->second->clear();
		delete it->second;
		it->second = nullptr;
	}

	_assignamentsIndexesMap.clear();

	/*for (std::vector< std::pair<AssignType, std::list<BinaryProperty *> *>>::iterator it = _assignaments.begin(); it != _assignaments.end(); ++it) {
		for (std::list<BinaryProperty *>::iterator it_list = (*it).second->begin(); it_list != (*it).second->end(); ++it_list)
			delete (*it_list);
		
		(*it).second->clear();
		delete (*it).second;
	}

	_assignaments.clear();	*/
	

}