

#ifndef _SPHERE_H_
#define _SPHERE_H_


#include "primitive.h"

class Sphere : public Primitive {

private:

	float _radius;
	float _radius2; // squared radius

protected:

	vec2 computeTexCoord(vec3 & point);
	

public:
	Sphere();
	Sphere(float radius, vec3 position);

	bool intersects(const Ray & ray, RaycastHit & hit, float & t) override;
	void computeSurfaceData(RaycastHit & hit) override;

	void updateTransform() override;

	void setRadius(float r);
	float getRadius();

};

#endif