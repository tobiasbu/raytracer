
#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_


#include "math/vec3.h"
#include "math/mat4.h"
#include "math/matrixtransform.h"

class Transform {

protected:

	// world to object matrix
	mat4 matrix;
	mat4 inversedMatrix;

	void calculateVectors() 
	{
		
		right = vec3(matrix.m[0], matrix.m[1], matrix.m[2]).normalized();
		up = vec3(matrix.m[4], matrix.m[5], matrix.m[6]).normalized();
		forward = vec3(matrix.m[8], matrix.m[9], matrix.m[10]).normalized();
			//vec3::cross(right, up);

		//std::cout << up << right << forward << std::endl;
	}

public:

	// position
	vec3 position;
	// euler angle
	vec3 angle;
	// scale
	vec3 scale;

	//vec3 origin;

	// y axis
	vec3 up; 
	// x axis
	vec3 right; 
	// z axis
	vec3 forward; 

	Transform() {
		position = vec3(0);
		angle = vec3(0, 0, 0);
		scale = vec3(1, 1, 1);
		up = vec3(0, 1, 0);
		forward = vec3(0, 0, 1);
		right = vec3(1, 0, 0);
	}

	/*virtual void preupdateTransform(const mat4 & viewMatrix) {


		mat4 s = MatrixTransform::scale(mat4::identity, scale);
		mat4 t = MatrixTransform::translate(mat4::identity, position);
		// x and y are swapped
		mat4 r = MatrixTransform::rotate_x_deg(mat4::identity, angle.x);
		r = MatrixTransform::rotate_y_deg(r, angle.y - 180);
		r = MatrixTransform::rotate_z_deg(r, angle.z);



		matrix = t; // *r * s;

		calculateVectors();

		//  precompute transformed object = model * view matrix
		matrix = matrix * viewMatrix;

		//matrix = t * r * s;  

		// computei inversed matrix
		inversedMatrix = mat4::inverse(matrix);
		
	}*/

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
		matrix = t * s; // * r);  

		// compute inversed matrix
		inversedMatrix = mat4::inverse(matrix);
		calculateVectors();



	}

	const mat4 & getMatrix()
	{
		return matrix;
	}

	const mat4 & getInversedMatrix() 
	{
		return inversedMatrix;
	}
};

#endif