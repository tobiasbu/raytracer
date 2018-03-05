
#ifndef _VECTOR2_H_
#define _VECTOR2_H_

#include "math/vec3.h"

class vec2 {

public:


	// Internal data
	float x, y;
	
	// Constructors

	vec2();
	vec2(const vec2 & v);
	vec2(const vec3 & v);
	vec2(const vec4 & v);
	vec2(float x);
	vec2(float x, float y);

	// Setters

	void set(float scalar);
	void set(float x, float y);
	void add(const vec2 &);
	void mult(const vec2 &);
	void div(const vec2 &);
	void sub(const vec2 &);
	vec2 mod(const vec2 & other);
	void negate();
	void normalize();

	// Getter Functions

	float magnitude();
	vec2 normalized();
	vec2 negative();
	

	// Static Functions

	static float distance(const vec2 & v1, const vec2 & v2);

	const static vec2 zero;


	// Boolean Operators

	bool operator ==(const vec2 & other);
	bool operator !=(const vec2 & other);

	// Relational Operators

	bool operator<(const vec2 & other) const;
	bool operator<=(const vec2 & other) const;
	bool operator>(const vec2 & other) const;
	bool operator>=(const vec2 & other) const;

	// Unary Binary

	// =

	vec2 & operator =(const vec2 & other);
	vec2 & operator =(float scalar);

	// + -

	vec2 operator+ (vec2 const & other);
	vec2 operator+ (float other);
	vec2 operator- (float other);

	// * /

	vec2 operator* (vec2 const & other);
	vec2 operator* (float other);
	vec2 operator/ (vec2 const & other);
	vec2 operator/ (float other);

	// += -=

	vec2 & operator+= (vec2 const & other);
	vec2 & operator-= (vec2 const & other);
	vec2 & operator+= (float other);
	vec2 & operator-= (float other);

	// *= /=

	vec2 & operator*= (vec2 const & other);
	vec2 & operator/= (vec2 const & other);
	vec2 & operator*= (float other);
	vec2 & operator/= (float other);

	// Array Subscriptor

	float & operator[](const size_t index) {

		assert(index >= 0 && index < 2 && "Can not access Vector2 element.");

		return (&x)[index];

	}

	const float & operator[](const size_t index) const {

		assert(index >= 0 && index < 2 && "Can not access Vector2 element.");

		return (&x)[index];

	}

	friend std::ostream& operator << (std::ostream &s, const vec2 &v)
	{
		return s << "vec2(\t" << v.x << "\t" << v.y << ')';
	}

};

vec2 operator -(vec2 const & v1, vec2 const & v2);


#endif