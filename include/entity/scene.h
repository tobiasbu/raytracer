
#ifndef _SCENE_H_
#define _SCENE_H_

#include "entity/primitive.h"
#include "entity/light.h"
#include "render/texture.h"
#include "entity/cubemap.h"
#include <vector>
#include <unordered_map>

class Scene {

private:

	Color _ambientLight;
	Color _backgroundColor;
	Material * _defaultMaterial;

	uint32_t _primitivesCount;
	uint32_t _meshesCount;
	uint32_t _trianglesCount;

	Bounds _worldAABB;
	uint32_t _farAABBIndex;
	uint32_t _closeAABBIndex;

	Texture * _backgroundImage = nullptr;
	vec2 tile = vec2(1, 1);
	CubeMap * _cubeMap;

	std::vector<Primitive *> _objects;
	std::vector<Light *> _lights;

	// TODO Resource Manager
	// But for this version it's okay just have unorded_map of material and texture
	// std::string is the name (filepath) of the resource
	std::unordered_map<std::string, Material *> _materials;
	std::unordered_map<std::string, Texture *> _textures;

public:

	Scene();
	~Scene();

	// add functions
	void addPrimitive(Primitive * object);
	void addLight(Light * light);
	void addMaterial(std::string name, Material * material);
	void addTexture(std::string name, Texture * texture);

	// background color
	void setAmbientColor(Color color);
	void setBackground(Color color);
	void setBackgroundImage(Texture * tex);
	void setBackgroundImageTile(const vec2 & tile);

	void setCubeMap(CubeMap * cubemap);

	Color getAmbientColor();
	Color getBackgroundColor(const vec3 & direction);
	const CubeMap * getCubeMap();
	Material * getDefaultMaterial();

	// getters
	Texture * getTexture(const std::string & name);
	Material * getMaterial(const std::string & name);
	std::vector<Primitive *> & getPrimitives();
	std::vector<Light *> & getLights();

	// functions
	
	uint32_t getTexturesCount() const
	{
		return _textures.size();
	}

	uint32_t getMaterialsCount() const
	{
		return _materials.size();
	}

	uint32_t getPrimitivesCount() const
	{
		return _primitivesCount;
	}

	uint32_t getMeshesCount() const
	{
		return _meshesCount;
	}

	uint32_t getTrianglesCount() const
	{
		return _trianglesCount;
	}

	const Bounds & getWorldBounds() const
	{
		return _worldAABB;
	}

	

	// clear data structures
	void clear();

	// update all object to world
	void updateTransforms();

	// update all object by view matrix (another solution)
	//void updateTransforms(const mat4 & viewMatrix);

	void countVariables(uint32_t & triangles, uint32_t & aabbs, uint32_t & prim);


};

#endif