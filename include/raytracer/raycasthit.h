

#ifndef _RAYCASTHIT_H_
#define _RAYCASTHIT_H_

//#include <float.h>
#include <limits.h>
#include "math/vec2.h"

#include "math/mathdef.hpp"


//const float fInfinity = std::numeric_limits<float>::max();
//const float fEpsilon = 1e-8f;

class Primitive;

//#define INFINITE std::numeric_limits<float>::max()
//#define EPSILON 1e-8

struct RaycastHit {


	Primitive * object; // hit object
	float distance; // t - distance to hit point
	vec3 point; // hit point
	vec2 texCoord; // texture coord (u,v)
	vec2 barycentric; // barycentric coordinate the hitted triangles.
	vec3 normal; // normal vector - shade
	vec3 tangent; // for bumping map
	const Ray * ray; // pointer to ray that has initial intersected
	// triangle index
	unsigned int triangle;
	unsigned int tests;

	RaycastHit()
		:
		distance(Infinity),
		point(0,0,0),
		object(nullptr),
		ray(nullptr),
		tests(0)
	{

	}

	RaycastHit(const RaycastHit & hit)
	{
		distance = hit.distance;
		object = hit.object;
		normal = hit.normal;
		point = hit.point;
	}



	RaycastHit(Primitive * object, const vec3 & normal, const vec3 & point, float t)
	{
		this->object = object;
		this->normal = normal;
		this->point = point;
		this->distance = t;
		
	}

	// Set when ocurrs a intersection with a object
	void setIntersecting(Primitive * object, const vec3 & point, float t)
	{
		this->object = object;
		this->point = point;
		this->distance = t;
	}

	RaycastHit & operator=(const RaycastHit & rhs)
	{
		this->object = rhs.object;
		this->point = rhs.point;
		this->distance = rhs.distance;
		this->normal = rhs.normal;
		this->texCoord = rhs.texCoord;
		this->barycentric = rhs.barycentric;
		this->tangent = rhs.tangent;

		return *this;
	}

	/*void set(vec3 & normal, vec3 & point, float t) 
	{
		this->normal = normal;
		this->point = point;
		this->distance = t;
	}*/

	~RaycastHit()
	{
		object = nullptr;
	}
		
};

#endif