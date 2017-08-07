
#include "entity/scene.h" 
#include "render/mesh.h"

Scene::Scene() 
{
	_backgroundColor = Color(0.035f,1);
	_ambientLight = Color(0.035f, 1);
	_defaultMaterial = new Material;
	_cubeMap = nullptr;
	_meshesCount = 0;
	_trianglesCount = 0;
	_primitivesCount = 0;
}

Scene::~Scene() 
{
	clear();
}

void Scene::setAmbientColor(Color color)
{
	_ambientLight = color;
}

void Scene::setBackground(Color color) 
{
	_backgroundColor = color;
}

void Scene::setCubeMap(CubeMap * cubemap)
{
	_cubeMap = cubemap;
}

const CubeMap * Scene::getCubeMap()
{
	return _cubeMap;
}

Color Scene::getAmbientColor() 
{
	return _ambientLight;
}

Color Scene::getBackgroundColor(const vec3 & direction) 
{
	if (_cubeMap != nullptr) {
		Color col = _backgroundColor;
		_cubeMap->getTexel(direction, col);
		return col;
	} else
		return _backgroundColor;
}

Material * Scene::getDefaultMaterial()
{
	return _defaultMaterial;
}

void Scene::addMaterial(std::string name, Material * material)
{
	_materials.insert(std::make_pair(name, material));
}

void Scene::addTexture(std::string name, Texture * texture)
{
	_textures.insert(std::make_pair(name, texture));
}

void Scene::addPrimitive(Primitive * object) {

	if (object->getType() == Primitive::PrimitiveType::Mesh)
	{
		_meshesCount++;
		_trianglesCount += static_cast<Mesh*>(object)->getTriangles().size() / 3;
	}
	else
		_primitivesCount++;

	

	_objects.push_back(object);

}

void Scene::addLight(Light * light) {

	_lights.push_back(light);

}

void Scene::clear() {

	_backgroundColor = Color(0.02f, 1);
	_ambientLight = Color(0.04f, 1);

	// clear objects, lights, materials and textures

	_meshesCount = 0;
	_trianglesCount = 0;
	_primitivesCount = 0;

	if (_cubeMap != nullptr)
	delete _cubeMap;
	_cubeMap = nullptr;


	for (std::unordered_map<std::string, Material *>::iterator it = _materials.begin(); it != _materials.end(); ++it)
		delete it->second;

	_materials.clear();

	for (std::unordered_map<std::string, Texture *>::iterator it = _textures.begin(); it != _textures.end(); ++it)
		delete it->second;

	_textures.clear();

	for (std::vector< Primitive * >::iterator it = _objects.begin(); it != _objects.end(); ++it) 
	{
		if ((*it)->getType() == Primitive::PrimitiveType::Mesh)
		{
			static_cast<Mesh*>((*it))->clear();                     
		}
		delete (*it);
	}

	_objects.clear();

	for (std::vector< Light * >::iterator it = _lights.begin(); it != _lights.end(); ++it)
		delete (*it);

	_lights.clear();

	



	/*i = _lights.size() - 1;

	while (i != 0) {
		delete _lights[i];
		_lights.pop_back();
		i--;
	}*/
}



//1# SOLUTION
//Compute all object transfrom multiplying model matrix by view matrix
/*void Scene::updateTransforms(const mat4 & viewMatrix)
{

	for (std::vector< Primitive * >::iterator it = _objects.begin(); it != _objects.end(); ++it) {
		// pass viewMatrix (camera matrix)
		(*it)->preupdateTransform(viewMatrix);
		//std::cout << (*it)->getMatrix() << std::endl;
	}
}*/


//2# SOLUTION
void Scene::updateTransforms()
{
	int index = 0;

	for (std::vector< Primitive * >::iterator it = _objects.begin(); it != _objects.end(); ++it) 
	{
		(*it)->updateTransform();

		const Bounds & b = (*it)->getBounds();

		_worldAABB = Bounds::merge(_worldAABB, b);
		//_worldAABB.extend(b);

		if (b.getMax() > _worldAABB.getMax())
			_farAABBIndex = index;

		if (b.getMin() < _worldAABB.getMin())
			_closeAABBIndex = index;

		if ((*it)->getType() == Primitive::PrimitiveType::Mesh)
			static_cast<Mesh*>((*it))->buildBVH();
			
	}
}

Texture * Scene::getTexture(const std::string & name)
{
	std::unordered_map< std::string, Texture * >::iterator it = _textures.find(name);
	
	if (it != _textures.end())
		return it->second;
	else
		return nullptr;
}

Material * Scene::getMaterial(const std::string & name)
{
	std::unordered_map< std::string, Material * >::iterator it = _materials.find(name);

	if (it != _materials.end())
		return it->second;
	else
		return nullptr;
}

std::vector<Primitive *> & Scene::getPrimitives() {

	return _objects;

}
std::vector<Light *> & Scene::getLights() {

	return _lights;

}

void Scene::countVariables(uint32_t & triangles, uint32_t & aabbs, uint32_t & prim)
{
	for (std::vector< Primitive * >::iterator it = _objects.begin(); it != _objects.end(); ++it)
	{
		if ((*it)->getType() == Primitive::PrimitiveType::Mesh)
		{
			static_cast<Mesh*>(*it)->countVariables(triangles, aabbs, prim);
		}
		else
			continue;
	}
}