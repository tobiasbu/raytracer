

#ifndef _PLANE_H_
#define _PLANE_H_

#include "primitive.h"

class Plane : public Primitive {


private:

	vec3 _normal;
	float _offset;

protected:

	


public:
	Plane();
	Plane(vec3 normal, float size);

	void updateTransform();

	void setNormal(const vec3 &);
	void setOffset(const float &);
	bool intersects(const Ray & ray, RaycastHit & hit, float & t);
	void computeSurfaceData(RaycastHit & hit);
	
	vec3 getNormal();

};

#endif