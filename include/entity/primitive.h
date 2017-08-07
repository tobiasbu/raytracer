

#ifndef _PRIMITIVE_H_
#define _PRIMITIVE_H_

#include "transform.h"
#include "raytracer/ray.h"
#include "raytracer/raycasthit.h"
#include "math/bounds.h"

#include "render/material.h"

#include <memory>


class Primitive : public Transform {



public:

	enum PrimitiveType {
		None,
		Plane,
		_Triangle,
		Cube,
		Cylinder,
		Sphere,
		Torus,
		Mesh
	};

	//void setName(const std::string & name);
	void setMaterial(Material *);

	const Bounds & getBounds() const;
	Material * getMaterial() const;
	PrimitiveType getType() const;
	//const std::string & getName() const;

	virtual bool intersects(const Ray & ray, RaycastHit & hit, float & t) = 0; 
	virtual void computeSurfaceData(RaycastHit & hit) = 0;
	

protected:

	//std::string _name = "unnamed";
	Material * _material = nullptr;
	Bounds _bounds;
	PrimitiveType _type = PrimitiveType::None;
	
	Primitive()
		:
		_material(nullptr)
	{

	}
	
	
	
	


};

#endif