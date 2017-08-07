
#ifndef _MATRIX4_H_
#define _MATRIX4_H_


#include "math/vec3.h"
#include "math/vec4.h"
#include <iostream>


/*
collum major order:
0 4 8  12
1 5 9  13
2 6 10 14
3 7 11 15

row major order:
we stored like this:
0  1  2  3 
4  5  6  7
8  9  10 11
12 13 14 15


*/

// Row-major order Matrix4x4f
class mat4 {

public:

	// Internal data
	float m[16];

	// Constructors
	mat4();
	mat4(float identity);
	mat4(float a, float b, float c, float d,
		float e, float f, float g, float h,
		float i, float j, float k, float l,
		float mm, float n, float o, float p);

	// Static Variables

	//const static mat4 one;
	const static mat4 zero;
	const static mat4 identity;

	// Static Functions

	//const vec3 multPoint() const;


	static float determinant(const mat4& mm);
	static mat4 transpose(const mat4& mm);
	static mat4 multiply(const mat4 & a, const mat4 & b);
	static mat4 inverse(const mat4 & a);


	// Functions

	mat4 transposed();

	mat4 invert();

	void set(float a, float b, float c, float d,
		float e, float f, float g, float h,
		float i, float j, float k, float l,
		float mm, float n, float o, float p);

	//vec4 operator* (const vec4& rhs);
	//mat4 operator* (const mat4& other);

	const float& operator () (int i, int j) const;

	float& operator () (int i, int j);

	mat4& operator= (const mat4& oter);
	
	float & operator[](const size_t index) {

		assert(index >= 0 && index < 16 && "Can not access Matrix4 element.");

		return m[index];

	}

	const float & operator[](const size_t index) const {

		assert(index >= 0 && index < 16 && "Can not access Matrix4 element.");

		return m[index];

	}

	friend std::ostream& operator << (std::ostream &s, const mat4 & mat)
	{
		return s << "mat4(\n" << mat.m[0] << ", " << mat.m[1] << ", " << mat.m[2] << ", " << mat.m[3] << '\n'
			<< mat.m[4] << ", " << mat.m[5] << ", " << mat.m[6] << ", " << mat.m[7] << '\n'
			 << mat.m[8] << ", " << mat.m[9] << ", " << mat.m[10] << ", " << mat.m[11] << '\n'
		     << mat.m[12] << ", " << mat.m[13] << ", " << mat.m[14] << ", " << mat.m[15] << "\n)";
	}	

};

mat4 operator* (mat4 const & a, mat4 const & b);
vec4 operator* (mat4 const & a, vec4 const & v);

#endif