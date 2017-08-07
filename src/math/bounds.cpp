
#include "math/bounds.h"
#include "math/maths.h"
#include <cmath>



Bounds::Bounds()
{
	// swaped values!
	min = std::numeric_limits<float>::max();
	max = std::numeric_limits<float>::lowest();
	size = max - min;
}

Bounds::Bounds(const Bounds & bounds)
	:
	min(bounds.min),
	max(bounds.max),
	size(bounds.size)
{

}

/*Bounds::Bounds(const Rect & rect) :
min(rect.position),
max(rect.position + rect.size),
size(rect.size),
rect(rect)
{

}*/

Bounds::Bounds(const vec3 & a, const vec3 & b)
	:
min(Math::min(a.x, b.x), Math::min(a.y, b.y), Math::min(a.z, b.z)),
max(Math::max(a.x, b.x), Math::max(a.y, b.y), Math::max(a.z, b.z)),
size(max-min)
/*rect(min, max - min)*/
{
}

Bounds::Bounds(const vec3 & v0, const vec3 & v1, const vec3 & v2)
{
	min.x = Math::min(v0.x, Math::min(v1.x, v2.x));
	min.y = Math::min(v0.y, Math::min(v1.y, v2.y));
	min.z = Math::min(v0.z, Math::min(v1.z, v2.z));

	max.x = Math::max(v0.x, Math::max(v1.x, v2.x));
	max.y = Math::max(v0.y, Math::max(v1.y, v2.y));
	max.z = Math::max(v0.z, Math::max(v1.z, v2.z));
	//max(Math::max(a.x, b.x), Math::max(a.y, b.y), Math::max(a.z, b.z)),
	size = max - min;
}

void Bounds::set(const vec3 & min, const vec3 & max) {
	this->min = min;
	this->max = max;
	size = (max - min);
	center = (min + max) / 2.0f;
}

Bounds & Bounds::extend(const vec3 & point)
{
	if (point.x < min.x) min.x = point.x;
	if (point.y < min.y) min.y = point.y;
	if (point.z < min.z) min.z = point.z;
	if (point.x > max.x) max.x = point.x;
	if (point.y > max.y) max.y = point.y;
	if (point.z > max.z) max.z = point.z;

	center = (min + max) / 2.0f;
	size = max - min;

	return *this;
}

Bounds & Bounds::extend(const Bounds & bounds)
{
	// min merge

	if (bounds.min.x < min.x)
		min.x = bounds.min.x;

	if (bounds.min.y < min.y)
		min.y = bounds.min.y;

	if (bounds.min.z < min.z)
		min.z = bounds.min.z;

	// max merge

	if (bounds.max.x > max.x)
		max.x = bounds.max.x;

	if (bounds.max.y > max.y)
		max.y = bounds.max.y;

	if (bounds.max.z > max.z)
		max.z = bounds.max.z;

	size = max - min;

	return *this;
}

Bounds Bounds::merge(const Bounds & b1, const Bounds & b2)
{
	Bounds ret;
	
	ret.min.x = Math::min(b1.min.x, b2.min.x);
	ret.min.y = Math::min(b1.min.y, b2.min.y);
	ret.min.z = Math::min(b1.min.z, b2.min.z);
	ret.max.x = Math::max(b1.max.x, b2.max.x);
	ret.max.y = Math::max(b1.max.y, b2.max.y);
	ret.max.z = Math::max(b1.max.z, b2.max.z);

	ret.size = ret.max - ret.min;

	return ret;

}

Bounds Bounds::merge(const Bounds & aabb, const vec3 & point)
{

	/*ret.min.x = Math::min(aabb.min.x, point.x);
	ret.min.y = Math::min(aabb.min.y, point.y);
	ret.min.z = Math::min(aabb.min.z, point.z);
	ret.max.x = Math::max(aabb.max.x, point.x);
	ret.max.y = Math::max(aabb.max.y, point.y);
	ret.max.z = Math::max(aabb.max.z, point.z);

	ret.size = ret.max - ret.min;*/

	return Bounds(
		vec3(Math::min(aabb.min.x, point.x), Math::min(aabb.min.y, point.y), Math::min(aabb.min.z, point.z)),
		vec3(Math::max(aabb.max.x, point.x), Math::max(aabb.max.y, point.y), Math::max(aabb.max.z, point.z))
	);
}

/// Getters

vec3 Bounds::getMin() const {

	return min;

}

vec3 Bounds::getMax() const {

	return max;

}

vec3 Bounds::getSize() const {

	return size;

}

float Bounds::area() const
{
	vec3 segments = max - min;//vec3::abs(min - max);
	//return 2 * (d.x * d.y + d.x * d.z + d.y * d.z);
	return (segments.x * segments.y + segments.x * segments.z + segments.y * segments.z) * 2.0f;
}

vec3 Bounds::offset(const vec3 & point) const
{
	vec3 o = point - min;

	if (max.x > min.x) 
		o.x /= max.x - min.x;

	if (max.y > min.y) 
		o.y /= max.y - min.y;

	if (max.z > min.z) 
		o.z /= max.z - min.z;

	return o;
}

uint8_t Bounds::getMaxAxis() const
{
	uint8_t idx = 0;

	if (size.y > size.x)
		idx = 1;

	if (size.z > size.y && size.z > size.x)
		idx = 2;

	return idx;
}

uint8_t Bounds::getMinAxis() const
{

	uint8_t idx = 0;

	if (size.y < size.x)
		idx = 1;

	if (size.z < size.y && size.z < size.x)
		idx = 2;

	return idx;
}

vec3 Bounds::centroid() const {

	return (0.5f * min + 0.5f * max);
		//(max + min) / 2.0f;

}

bool Bounds::intersects(const Ray & invRay, const bool * sign, float & t) const {


	
	float tmin, tmax, tymin, tymax, tzmin, tzmax;
	const Bounds &bounds = *this;

	/*tmin = (this->operator[](sign[0]).x - invRay.origin.x) * invRay.direction.x;
	tmax = (this->operator[](1 - sign[0]).x - invRay.origin.x) * invRay.direction.x;
	tymin = (this->operator[](sign[1]).y - invRay.origin.y) * invRay.direction.y;
	tymax = (this->operator[](1 - sign[1]).y - invRay.origin.y) * invRay.direction.y;*/
	tmin = (bounds[sign[0]].x - invRay.origin.x) * invRay.direction.x;
	tmax = (bounds[1 - sign[0]].x - invRay.origin.x) * invRay.direction.x;
	tymin = (bounds[sign[1]].y - invRay.origin.y) * invRay.direction.y;
	tymax = (bounds[1 - sign[1]].y - invRay.origin.y) * invRay.direction.y;
	

	if ((tmin > tymax) || (tymin > tmax))
		return false;

	if (tymin > tmin)
		tmin = tymin;
	if (tymax < tmax)
		tmax = tymax;

	tzmin = (this->operator[](sign[2]).z - invRay.origin.z) * invRay.direction.z;
	tzmax = (this->operator[](1 - sign[2]).z - invRay.origin.z) * invRay.direction.z;

	if ((tmin > tzmax) || (tzmin > tmax))
		return false;

	if (tzmin > tmin)
		tmin = tzmin;
	if (tzmax < tmax)
		tmax = tzmax;

	t = tmin;

	if (tmax < tmin && tmax < 0.0)
		return false;

	

	return true;

	/*if (tmax > 0)
		return true;
	else
		return false;*/
	
	

}

bool Bounds::intersects(const Ray & ray, const vec3 & invDir, const int * dirIsNeg, float & t) const
{
	const Bounds &bounds = *this;
	// Check for ray intersection against $x$ and $y$ slabs
	float tMin = (bounds[dirIsNeg[0]].x - ray.origin.x) * invDir.x;
	float tMax = (bounds[1 - dirIsNeg[0]].x - ray.origin.x) * invDir.x;
	float tyMin = (bounds[dirIsNeg[1]].y - ray.origin.y) * invDir.y;
	float tyMax = (bounds[1 - dirIsNeg[1]].y - ray.origin.y) * invDir.y;

	// Update _tMax_ and _tyMax_ to ensure robust bounds intersection
	tMax *= 1 + 2 * Math::gamma(3);
	tyMax *= 1 + 2 * Math::gamma(3);

	if (tMin > tyMax || tyMin > tMax) 
		return false;

	if (tyMin > tMin) tMin = tyMin;
	if (tyMax < tMax) tMax = tyMax;

	// Check for ray intersection against $z$ slab
	float tzMin = (bounds[dirIsNeg[2]].z - ray.origin.z) * invDir.z;
	float tzMax = (bounds[1 - dirIsNeg[2]].z - ray.origin.z) * invDir.z;

	// Update _tzMax_ to ensure robust bounds intersection
	tzMax *= 1 + 2 * Math::gamma(3);

	if (tMin > tzMax || tzMin > tMax) 
		return false;

	if (tzMin > tMin) 
		tMin = tzMin;

	if (tzMax < tMax) 
		tMax = tzMax;

	//t = tMin;
	
	//return (tMax > 0);

	if ((tMin < t) && (tMax > 0))
	{
		t = tMin;
		return true;
	}
	else
		return false;

}

Bounds & Bounds::transform(const mat4 & mat) {

	vec3 pos = MatrixTransform::multiplyVec(mat, center);

	vec3 half = size / 2;
	half.x = fabs(half.x);
	half.y = fabs(half.y);
	half.z = fabs(half.z);

	min = pos - half;
	max = pos + half;

	//std::cout << size << std::endl;

	return *this;
}

/// Functions

/*bool Bounds::intersects(const Bounds & bounds) {

	return Bounds::intersects(*this, bounds);

}

bool Bounds::contains(const vec3 & point) {

	return Bounds::contains(*this, point);

}*/


/*void Bounds::calculate(const Renderer * renderer) {


}

void Bounds::calculate(const Vector2f & anchor, const Vector2f & pos, const Vector2f & scale, const Vector2f & size, float angle, float depth) {


}*/

/// Static Functions

/*bool Bounds::intersects(const Bounds & a, const Bounds & b) {

	return true;

}

bool Bounds::contains(const Bounds & bounds, const vec3 & point) {

	return true;

}*/
