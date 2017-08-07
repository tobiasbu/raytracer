


#ifndef _MATRIXTRANSFORM_H_
#define _MATRIXTRANSFORM_H_

#include "math/mat4.h"


namespace MatrixTransform {

	/*--------------------------AFFINE MATRIX FUNCTIONS---------------------------*/
	// translate a 4d matrix with xyz array
	mat4 translate(const mat4& m, const vec3& v);

	// rotate around x axis by an angle in degrees
	mat4 rotate_x_deg(const mat4& m, float deg);

	// rotate around y axis by an angle in degrees
	mat4 rotate_y_deg(const mat4& m, float deg);

	// rotate around z axis by an angle in degrees
	mat4 rotate_z_deg(const mat4& m, float deg);

	//TODO
	//mat4 rotation(const mat4 & m, const vec3 & axis, float radians);

	// scale a matrix by [x, y, z]
	mat4 scale(const mat4& m, const vec3& v);

	// -- multDirMatrix
	// multiply matrix by vec3 (direction components)
	// usage example : view matrix by directional vector
	// return new direction based on view matrix
	vec3 multiplyPoint(const mat4 & m, const vec3 & src);

	// -- multVecMatrix
	// multiply matrix by vec3
	vec3 multiplyVec(const mat4 & m, const vec3 & v);

	mat4 perspective(float fovy, float aspect, float near, float far);

	mat4 ortho(float left, float right, float bottom, float top, float near, float far);

	mat4 lookAt(const vec3 & eye, const vec3 & center, const vec3 & up = vec3(0,1,0));
	//mat4 lookAt2(const vec3 & eye, const vec3 & center, const vec3 & up = vec3(0, 1, 0));

	//  Setters

	// set direction to matrix.  (previous calculate dir)
	void setDirection(mat4 & a, const vec3 & dir);
	// set position to matrix
	void setPosition(mat4 & a, const vec3 & pos);
	// set up vector to matrix
	void setUp(mat4 & a, const vec3 & dir, const vec3 & up);
}

#endif