
#include "math/vec2.h"
#include "math/vec3.h"
#include "math/maths.h"

// Parameterization of implicit surfaces
namespace TextureMapping {

	vec2 spherical(const vec3 & point, const vec3 & center, const float & radius)
	{
		//tex.x = (1 + atan2(nhit.z, nhit.x) / M_PI) * 0.5;
		// tex.y = acosf(nhit.y) / M_PI;

		// u = 0.5 + atan2(d.z, d.x) / M_PI * 0.5;
		// v = 0.5 - asin(d.y) / M_PI;

		float theta = acos((point.z - center.z) / radius);
		float phi = atan2(point.y - center.y, point.x - center.x);

		if (phi < 0)
			phi = phi + 2 * PI;


		float u = phi / (2 * PI);
		float v = (PI - theta) / PI;

		// tc.print();
		return vec2(u, v);
	}

	inline vec2 planar(const vec3 & point, const vec3 & normal/*, const float & offset*/) 
	{
		vec3 uAxis = vec3(normal.y, normal.z, normal.x);
		vec3 vAxis = vec3::cross(uAxis, normal);



		return vec2(vec3::dot(point, uAxis), vec3::dot(point, vAxis));
	}

	// TODO
	vec2 cylindrical(const vec3 & point, const float & radius, const float & height)
	{
		return vec2();
	}

};