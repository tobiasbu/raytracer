
#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include "primitive.h"
#include "math/vec3.h"
#include "math/vec2.h"

class Triangle : public Primitive
{
	friend class Mesh;

private:

	uint32_t _index = 0;
	vec3 _vertices[3];
	vec3 _normals[3];
	vec2 _texcoords[3];
	vec3 _normal;
	vec3 _tangent;
	bool hasNormals = false;

	// for mesh initializer
	void initialize();

public:

	Triangle()
	{
		_type = PrimitiveType::_Triangle;
		_material = nullptr;
	}

	Triangle(const uint32_t & index, vec3 vertices[3], vec2 * texCoord, vec3 * normals, vec3 * tangent);

	void updateTransform() override;
	void updateTransform(const mat4 & matrix, const mat4 & inversedMatrix);
	bool intersects(const Ray & ray, RaycastHit & hit, float & t) override;
	void computeSurfaceData(RaycastHit & hit) override;
};

#endif