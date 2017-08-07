

#ifndef _MATRIX3_H_
#define _MATRIX3_H_

#include <iostream>
#include <assert.h>


/*
COLLUM ORDER:
0 3 6
1 4 7
2 5 8

ROW ORDER

0 1 2
3 4 5
6 7 8
*/

class vec3;

// Row-major order Matrix3x3f
class mat3 {

public:

	// Internal data
	float m[9];

	mat3();

	mat3(float identity);

	mat3(float a, float b, float c,
		float d, float e, float f,
		float g, float h, float i);

	mat3(vec3 const & v0, vec3 const & v1, vec3 const &);

	// Functions

	void set(float a, float b, float c,
		float d, float e, float f,
		float g, float h, float i);

	static mat3 transpose(const mat3 & a);

	// Static

	const static mat3 identity;
	const static mat3 zero;

	// Operators

	const float& operator () (int i, int j) const;

	float& operator () (int i, int j);

	float & operator[](const size_t index) {

		assert(index >= 0 && index < 9 && "Can not access Matrix3 element.");

		return m[index];

	}

	const float & operator[](const size_t index) const {

		assert(index >= 0 && index < 9 && "Can not access Matrix3 element.");

		return m[index];

	}

};

mat3 operator* (mat3 const & a, mat3 const & b);
vec3 operator* (mat3 const & a, vec3 const & v);

#endif