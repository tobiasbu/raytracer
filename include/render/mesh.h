
#ifndef _MESH_H_
#define _MESH_H_

#include "entity\triangle.h"
#include "entity\primitive.h"
#include "accel\BVH.h"
#include "render\objloader.h"
#include <vector>

class Mesh : public Primitive {

private:

	

	uint32_t _numTriangles = 0;
	std::vector<vec3> _vertices;
	std::vector<vec3> _normals;
	std::vector<vec2> _texCoords;
	std::vector<vec3> _tangents;
	std::vector<uint32_t> _triangleIndex;

	int _useBVH = 0;
	BVH * _bvh;
	std::vector<Primitive*> _triangles;
	
	bool rayTriangleIntersects(const Ray & ray, const vec3 & v0, const vec3 & v1, const vec3 & v2, float & t, float & u, float & v) const;

public:

	

	Mesh() 
	{
		_type = PrimitiveType::Mesh;
	}



	Mesh(
		const std::vector<uint32_t> & triangleIndex,
		/*std::vector<uint32_t> & vertsIndex,*/
		const std::vector<vec3> & vertices,
		const std::vector<vec3> & normals,
		const std::vector<vec2> & texCoords,
		const int & useBVH = 0
		);

	//Mesh(std::vector<ObjLoader::Vertex> & vertexes, std::vector<uint32_t> & vertsIndex);


	void clear()
	{
		_bvh->clear();
		delete _bvh;
	}

	const std::vector<vec3> & getVertices() const;
	const std::vector<uint32_t> & getTriangles() const
	{
		return _triangleIndex;
	}

	std::vector<Primitive*> & getTrianglesPrimitives()
	{
		return _triangles;
	}

	bool intersects(const Ray & ray, RaycastHit & hit, float & t);
	
	void computeSurfaceData(RaycastHit & hit);
	
	void updateTransform() override;

	bool computeTangents();

	void buildBVH();

	void countVariables(uint32_t & triangles, uint32_t & aabbs, uint32_t & prim)
	{
		triangles += _bvh->getTriangleTests();
		aabbs += _bvh->getAABBTests();
		prim += _bvh->getIntersectionTests();
	}


};

#endif