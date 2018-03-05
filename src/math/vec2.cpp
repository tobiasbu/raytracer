
#include "math/vec2.h"
#include "math/vec4.h"
#include "math\maths.h"
#include <math.h>

const vec2 vec2::zero = vec2(0.0);

vec2::vec2() {

	x = 0;
	y = 0;

}

vec2::vec2(const vec2 & v) {

	x = v.x;
	y = v.y;

}

vec2::vec2(const vec3 & v) {

	x = v.x;
	y = v.y;

}

vec2::vec2(const vec4 & v) {

	x = v.x;
	y = v.y;

}

vec2::vec2(float x) {

	this->x = x;
	this->y = x;

}

vec2::vec2(float x, float y) {

	this->x = x;
	this->y = y;

}

// Setters

void vec2::set(float scalar) {

	this->x = scalar;
	this->y = scalar;

}

void vec2::set(float x, float y) {

	this->x = x;
	this->y = y;

}

void vec2::add(const vec2 & v) {

	x += v.x;
	y += v.y;

}

void vec2::mult(const vec2 & v) {

	x *= v.x;
	y *= v.y;

}

void vec2::div(const vec2 & v) {

	x /= v.x;
	y /= v.y;

}

void vec2::sub(const vec2 & v) {

	x -= v.x;
	y -= v.y;

}

vec2 vec2::mod(const vec2 & other)
{
	this->x = std::fmodf(this->x, other.x); // other.x;
	this->y = std::fmodf(this->y, other.y);
	return *this;
}

void vec2::negate() {

	x = -x;
	y = -y;

}

void vec2::normalize() {

	float magnitude = this->magnitude();
	x /= magnitude;
	y /= magnitude;

}

// Getter Functions

float vec2::magnitude() {

	return sqrt((x*x) + (y*y));

}

vec2 vec2::normalized() {

	float magnitude = this->magnitude();
	return vec2(x / magnitude, y / magnitude);
}

vec2 vec2::negative() {

	return vec2(-x, -y);

}

// Static Functions

float vec2::distance(const vec2 & v1, const vec2 & v2) {

	return sqrt(pow(v1.x - v2.x, 2) + pow(v1.y - v2.y, 2));

}

// Boolean Operators

bool vec2::operator ==(const vec2 & other) {

	return (x == other.x) && (y == other.y);

}

bool vec2::operator !=(const vec2 & other) {

	return (x != other.x) || (y != other.y);

}
 

// Relational Operators

bool vec2::operator<(const vec2 & other) const {

	return (this->x < other.x) && (this->y < other.y);

}

bool vec2::operator<=(const vec2 & other) const {

	return (this->x <= other.x) && (this->y <= other.y);

}

bool vec2::operator>(const vec2 & other) const {

	return (this->x > other.x) && (this->y > other.y);

}

bool vec2::operator>=(const vec2 & other) const {

	return (this->x >= other.x) && (this->y >= other.y);

}

// Unary Binary

// =

vec2 & vec2::operator =(const vec2 & other) {

	this->x = other.x;
	this->y = other.y;
	return *this;

}

vec2 & vec2::operator =(float scalar) {

	this->x = scalar;
	this->y = scalar;
	return *this;

}

// + -

vec2 vec2::operator+ (vec2 const & other) {

	vec2 v(x + other.x, y + other.y);
	return v;

}

vec2 vec2::operator+ (float other) {

	vec3 v(x + other, y + other);
	return v;

}

vec2 vec2::operator- (float other) {

	vec3 v(x - other, y - other);
	return v;

}

// * /

vec2 vec2::operator* (vec2 const & other) {

	vec2 v(x * other.x, y * other.y);
	return v;


}

vec2 vec2::operator* (float other) {

	vec2 v(x * other, y * other);
	return v;

}

vec2 vec2::operator/ (vec2 const & other) {

	vec3 v(x / other.x, y / other.y);
	return v;

}

vec2 vec2::operator/ (float other) {

	vec2 v(x / other, y / other);
	return v;

}

// += -=

vec2 & vec2::operator+= (vec2 const & other) {

	this->x += other.x;
	this->y += other.y;
	return *this;

}

vec2 & vec2::operator-= (vec2 const & other) {

	this->x -= other.x;
	this->y -= other.y;
	return *this;

}

vec2 & vec2::operator+= (float other) {

	this->x += other;
	this->y += other;
	return *this;

}

vec2 & vec2::operator-= (float other) {

	this->x -= other;
	this->y -= other;
	return *this;

}

// *= /=

vec2 & vec2::operator*= (vec2 const & other) {

	this->x *= other.x;
	this->y *= other.y;
	return *this;

}

vec2 & vec2::operator/= (vec2 const & other) {

	this->x /= other.x;
	this->y /= other.y;
	return *this;

}

vec2 & vec2::operator*= (float other) {

	this->x *= other;
	this->y *= other;
	return *this;
}

vec2 & vec2::operator/= (float other) {
	this->x /= other;
	this->y /= other;
	return *this;
}

vec2 operator -(vec2 const & v1, vec2 const & v2) {

	return vec3(v1.x - v2.x, v1.y - v2.y);

}