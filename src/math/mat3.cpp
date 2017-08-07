
#include "math\mat3.h"
#include "math/vec3.h"

const mat3 mat3::identity(1.0);
const mat3 mat3::zero(0.0);

mat3::mat3()
{
	set(0, 0, 0, 0, 0, 0, 0, 0, 0);
}

mat3::mat3(float a) 
{
	/*
	1 0 0
	0 1 0
	0 0 1
	*/

	m[0] = a;
	m[1] = 0;
	m[2] = 0;

	m[3] = 0;
	m[4] = a;
	m[5] = 0;

	m[6] = 0;
	m[7] = 0;
	m[8] = a;

}

mat3::mat3(float a, float b, float c,
	float d, float e, float f,
	float g, float h, float i) 
{
	set(a, b, c, d, e, f, g, h, i);
}

mat3::mat3(const vec3 & v0, const vec3 & v1, const vec3 & v2)
{
	set(v0.x, v0.y, v0.z,
		v1.x, v1.y, v1.z,
		v2.x, v2.y, v2.z
		);
}

void mat3::set(float a, float b, float c,
	float d, float e, float f,
	float g, float h, float i)
{
	m[0] = a;
	m[1] = b;
	m[2] = c;
	m[3] = d;
	m[4] = e;
	m[5] = f;
	m[6] = g;
	m[7] = h;
	m[8] = i;
}


mat3 mat3::transpose(const mat3 & a)
{
	return mat3(
		a.m[0], a.m[3], a.m[6],
		a.m[1], a.m[4], a.m[7],
		a.m[2], a.m[5], a.m[8]
		);
}

const float& mat3::operator() (int i, int j) const 
{
	// col = j * 3 + i == i + j * 3
	// row = j + i * 3
	return m[j + i * 3];
}

float& mat3::operator() (int i, int j) 
{
	return m[j + i * 3];
}

mat3 operator* (mat3 const & a, mat3 const & b)
{

	mat3 c; // zeroes

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			for (int k = 0; k < 3; ++k)
			{
				c(i, k) += a(i, j) * b(j, k);
			}
		}
	}

	return c;

}

vec3 operator* (mat3 const & a, vec3 const & v)
{
	vec3 out;

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			out[i] += v[j] * a(i, j);
		}
	}

	return out;
}
