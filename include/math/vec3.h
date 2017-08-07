
#ifndef _VECTOR3_H_
#define _VECTOR3_H_

#include <assert.h>
#include <iostream>

class vec2;
class vec4;
struct Color;

class vec3 {

public:


	// Internal data
	float x, y, z;


	// Constructors

	vec3();
	vec3(const vec2 & v);
	vec3(const vec3 & v);
	vec3(const vec4 & v);
	vec3(const Color & color);
	vec3(float scalar);
	vec3(float x, float y);
	vec3(float x, float y, float z);

	// Setters

	void set(float scalar);
	void set(float x, float y, float z);
	
	void add(const vec3 &);
	void mult(const vec3 &);
	void div(const vec3 &);
	void sub(const vec3 &);
	void negate();
	void normalize();

	// Getter Functions

	// lenght of vector
	float norm() const;

	// sqrt length of vector
	float magnitude();

	// max value between vec3 components
	float maxComponent();


	// return the normal of vector
	vec3 normalized() const;
	vec3 negative();

	// Conversion Functions

	void set(const Color & color);
	Color toColor();

	// Static Functions

	static vec3 abs(const vec3 & v);
	static float dot(const vec3 & v1, const vec3 & v2);
	static vec3 cross(const vec3 & v1, const vec3 & v2);
	static float distance(const vec3 & v1, const vec3 & v2);

	// angle between two vectors. return in radians
	static float angle(const vec3 & v1, const vec3 & v2);

	// Compute reflection direction 
	static vec3 reflect(const vec3 & i, const vec3 & n);

	// Compute refraction direction 
	static vec3 refract(const vec3 & i, const vec3 & n, const float & ior);

	static vec3 lerp(const vec3 & from, const vec3 & to, float t);

	//static vec3 normalize(const vec3 & v);

	// Static Variable

	const static vec3 zero;

	// Boolean Operators

	bool operator ==(const vec3 & other);
	bool operator !=(const vec3 & other);

	// Relational Operators

	bool operator<(const vec3 & other) const;
	bool operator<=(const vec3 & other) const;
	bool operator>(const vec3 & other) const;
	bool operator>=(const vec3 & other) const;

	// Unary Binary

	// =

	vec3 & operator =(const vec3 & other);
	vec3 & operator =(float scalar);

	// + -

	//vec3 operator+ (vec3 const & other);
	//vec3 operator+ (float other);
	//vec3 operator- (vec3 const & other);
	//vec3 operator- (float other);

	// * /

	//vec3 operator* (vec3 const & other);
	//vec3 operator* (float other);
	//vec3 operator/ (vec3 const & other);
	//vec3 operator/ (float other);

	// += -=

	vec3 & operator+= (vec3 const & other);
	vec3 & operator-= (vec3 const & other);
	vec3 & operator+= (float other);
	vec3 & operator-= (float other);

	// *= /=

	vec3 & operator*= (vec3 const & other);
	vec3 & operator/= (vec3 const & other);
	vec3 & operator*= (float other);
	vec3 & operator/= (float other);

	// Array Subscriptor

	float & operator[](const size_t index) {

		assert(index >= 0 && index < 3 && "Can not access Vector3 element.");

		return (&x)[index];

	}

	const float & operator[](const size_t index) const {

		assert(index >= 0 && index < 3 && "Can not access Vector3 element.");

		return (&x)[index];

	}

	friend std::ostream& operator << (std::ostream &s, const vec3 &v)
	{
		return s << "vec3(\t" << v.x << "\t" << v.y << "\t" << v.z << ')';
	}
	


};

// operators

vec3 operator+ (vec3 const & v1, vec3 const & v2);
vec3 operator- (vec3 const & v1, vec3 const & v2);
vec3 operator* (vec3 const & v1, vec3 const & v2);
vec3 operator/ (vec3 const & v1, vec3 const & v2);

// unary negate
vec3 operator- (vec3 const & v1);

// by scalar

vec3 operator+ (vec3 const & v, float f);
vec3 operator- (vec3 const & v, float f);
vec3 operator* (vec3 const & v, float f);
vec3 operator* (float f, vec3 const & v);
vec3 operator/ (vec3 const & v, float f);

#endif