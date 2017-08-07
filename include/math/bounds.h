


#ifndef _BOUNDS_H_
#define _BOUNDS_H_

#include "math/vec3.h"
#include "raytracer/ray.h"
#include <stdint.h>

// AABB - bounding box
struct Bounds {

private:

	vec3 min, max;


	vec3 size;
	vec3 center;
	//Rect rect;

public:

	Bounds();
	Bounds(const Bounds & bounds);
	Bounds(const vec3 & pointA, const vec3 & pointB);
	Bounds(const vec3 & v0, const vec3 & v1, const vec3 & v2);
	
	/// Setters
	void set(const vec3 & min, const vec3 & max);

	Bounds & extend(const Bounds & bounds);
	Bounds & extend(const vec3 & point);

	/// Getters

	vec3 getMin() const;
	vec3 getMax() const;
	vec3 getSize() const;
	vec3 centroid() const;
	float area() const;
	vec3 offset(const vec3 & point) const;
	// return index of the axis (x = 0, y = 1, z = 2)
	uint8_t getMaxAxis() const;
	uint8_t getMinAxis() const;

	/// Functions

	bool intersects(const Ray & ray, const bool * sign, float & t) const;
	bool intersects(const Ray & ray, const vec3 & inversedDirection, const int * dirIsNeg, float & t) const;
	Bounds & transform(const mat4 & mat);

	static Bounds merge(const Bounds & a, const Bounds & b);
	static Bounds merge(const Bounds & aabb, const vec3 & point);

	// infinity bounds
	//const static Bounds infinity;

	//bool intersects(const Bounds & bounds);
	//bool contains(const vec3 & point);



	/// Static Functions

	//static bool intersects(const Bounds & a, const Bounds & b);
	//static bool contains(const Bounds & bounds, const vec3 & point);


	const Bounds & operator=(const Bounds & rhs)
	{
		this->min = rhs.min;
		this->max = rhs.max;
		this->size = rhs.size;
		return *this;
	}

	vec3 & operator[](const size_t index) {

		assert(index >= 0 && index < 2 && "Can not access Bounds element.");

		return (index == 0) ? min : max;
			//(&min)[index];

	}

	const vec3 & operator[](const size_t index) const {

		assert(index >= 0 && index < 2 && "Can not access Bounds element.");

		return (index == 0) ? min : max;
		//return (&min)[index];

	}


};

#endif