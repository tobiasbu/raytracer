
#ifndef _LIGHT_H_
#define _LIGHT_H_


#include "render/color.h"
#include "math/vec3.h"
#include "math/mat4.h"
#include "entity/transform.h"
#include "math/mathdef.hpp"

class Light : public Transform
{

public:

	float intensity; // fallof
	Color color;
	int type = 0;

	Light()
		:
		color(Color::white),
		intensity(0.1f)
	{
		position = vec3(0, -1, -1);
	}
	virtual ~Light() {}

	virtual void illuminate(const vec3 & point, vec3 & lightDir, vec3 & lightColorIntensity, float & distance, float & radius) const = 0;

	virtual void updateTransform() {


		mat4 s = MatrixTransform::scale(mat4::identity, scale);
		mat4 t = MatrixTransform::translate(mat4::identity, position);
		mat4 r = MatrixTransform::rotate_x_deg(mat4::identity, angle.x);

		r = MatrixTransform::rotate_y_deg(r, angle.y);
		r = MatrixTransform::rotate_z_deg(r, angle.z);

		// transform o row-major is T * R * S
		//  collum:
		//matrix = s * r * t;

		// another solution: precompute transformed object: only model matrix
		matrix = t; // * r);  

		// compute inversed matrix
		inversedMatrix = mat4::inverse(matrix);
		calculateVectors();



	}
};

class DirectionalLight : public Light
{

public:

	vec3 direction;

	DirectionalLight();
	DirectionalLight(const vec3 & position, const vec3 & targetDirection, const Color & color = Color::white, float intensity = 4);
	void illuminate(const vec3 & point, vec3 & lightDir, vec3 & lightColorIntensity, float & distance, float & radius) const;
	//void updateTransform();

	void setDirection(const vec3 & dir);
};

class PointLight : public Light
{

public:

	float range = 1;

	PointLight();
	PointLight(const vec3 & position, const Color & color, float intensity = 4);

	void illuminate(const vec3 & point, vec3 & lightDir, vec3 & lightColorIntensity, float & distance, float & radius) const;
	//void updateTransform();
};



#endif