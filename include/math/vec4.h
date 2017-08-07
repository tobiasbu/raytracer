
#ifndef _VECTOR4_H_
#define _VECTOR4_H_

#include <assert.h>
#include <iostream>

class vec2;
class vec3;
struct Color;

class vec4 {

public:


	// Internal data
	float x, y, z, w;


	// Constructors

	vec4();
	vec4(const vec2 & v);
	vec4(const vec3 & v);
	vec4(const vec3 & v, float w);
	vec4(const vec4 & v);
	vec4(const Color & color);
	vec4(float x);
	vec4(float x, float y);
	vec4(float x, float y, float z);
	vec4(float x, float y, float z, float w);

	// Setters

	void set(float scalar);
	void set(float x, float y, float , float w);

	void add(const vec4 &);
	void mult(const vec4 &);
	void div(const vec4 &);
	void sub(const vec4 &);
	void negate();
	float norm() const;
	vec4 normalize();

	// Conversion Functions

	void set(const Color & color);
	Color toColor();

	// Static Functions
	const static vec4 zero;

	// Boolean Operators

	bool operator ==(const vec4 & other);
	bool operator !=(const vec4 & other);

	// Relational Operators

	bool operator<(const vec4 & other) const;
	bool operator<=(const vec4 & other) const;
	bool operator>(const vec4 & other) const;
	bool operator>=(const vec4 & other) const;

	// Unary Binary

	// =

	vec4 & operator =(const vec4 & other);
	vec4 & operator =(float scalar);

	vec4 & operator+= (vec4 const & other);
	vec4 & operator-= (vec4 const & other);
	vec4 & operator+= (float other);
	vec4 & operator-= (float other);

	// *= /=

	vec4 & operator*= (vec4 const & other);
	vec4 & operator/= (vec4 const & other);
	vec4 & operator*= (float other);
	vec4 & operator/= (float other);

	// Array Subscriptor

	float & operator[](const size_t index) {

		assert(index >= 0 && index < 4 && "Can not access Vector4 element.");

		return (&x)[index];

	}

	const float & operator[](const size_t index) const {

		assert(index >= 0 && index < 4 && "Can not access Vector4 element.");

		return (&x)[index];

	}

	friend std::ostream& operator << (std::ostream &s, const vec4 &v)
	{
		return s << "vec4(\t" << v.x << "\t" << v.y << "\t" << v.z << "\t" << v.w << ')';
	}



};

// operators

vec4 operator+ (vec4 const & v1, vec4 const & v2);
vec4 operator- (vec4 const & v1, vec4 const & v2);
vec4 operator* (vec4 const & v1, vec4 const & v2);
vec4 operator/ (vec4 const & v1, vec4 const & v2);

// unary negate
vec4 operator- (vec4 const & v1);

// by scalar

vec4 operator+ (vec4 const & v, float f);
vec4 operator- (vec4 const & v, float f);
vec4 operator* (vec4 const & v, float f);
vec4 operator/ (vec4 const & v, float f);

#endif