
#ifndef _MATHS_H_
#define _MATHS_H_

#include "mathdef.hpp"
#include <math.h>
#include "math/vec3.h"


namespace Math {

	bool solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1);

	// linear interpolation
	float lerp(const float &  from, const float &  to, const float &  t);
	float unclampedLerp(float from, float to, float t);

	// billinear interpolation
	float blerp(const float & v00, const float & v10, const float & v01, const float & v11, const float & tx, const float & ty);

	// clamp value between min or max
	const float & clamp(const float & value, const float & min, const float & max);
	int clampint(const int & value, const int & min, const int & max);

	const float & max(const float & left, const float & right);
	const float & min(const float & left, const float & right);

	// normalize the given vector
	vec3 normalize(const vec3 &);
	float magnitude(const vec3 &);

	// float modulo
	float modulo(const float & x);

	// TODO
	float fmodulo(const float & x, const float & y);

	float degtorad(float);
	float radtodeg(float);

	float gamma(int n);

	float randomRange(const float & from, const float & to);

	// check if a point is inside a triangle
	bool inTriangle(const vec3 & v0, const vec3 & v1, const vec3 & v2, const vec3 & point);

	// compute normal by the given vertexes
	vec3 computeNormal(const vec3 & v0, const vec3 & v1, const vec3 & v2);

}

#endif