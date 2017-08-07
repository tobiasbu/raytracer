

#ifndef _RAY_H_
#define _RAY_H_


#include "math/vec3.h"
#include "math/vec4.h"
#include "math/mat4.h"
#include "math/matrixtransform.h"

struct Ray {

	vec3 origin;
	vec3 direction;

	Ray() 
	{
	}

	Ray(const Ray & ray) {

		this->origin = ray.origin;
		this->direction = ray.direction;

	}

	Ray(const vec3 & origin, const vec3 & direction) 
	{
		this->origin = origin;
		this->direction = direction;
	}

	void setDirection(const vec3 & target)
	{
		direction = (origin - target).normalized();
	}

	Ray & transform(const mat4 & m) {

		//origin = m * vec4(origin, 1);
		//direction = m * vec4(direction, 0);
		direction = MatrixTransform::multiplyPoint(m, direction);

		origin = MatrixTransform::multiplyVec(m, origin);

		return *this;
	}

	vec3 computeImpactPoint(float t) const
	{
		return origin + direction * t;
	}

	Ray & operator=(const Ray & ray) {

		this->origin = ray.origin;
		this->direction = ray.direction;
		return *this;

	}
};


#endif
