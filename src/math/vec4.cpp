
#include "math/vec4.h"
#include "math/vec3.h"
#include "math/vec2.h"
#include "render\color.h"
#include "math/maths.h"

const vec4 vec4::zero(0.0);


vec4::vec4() 
	:
	x(0),
	y(0),
	z(0),
	w(0)
{

}

vec4::vec4(const vec4 & v) {

	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
}

vec4::vec4(const vec2 & v) {

	x = v.x;
	y = v.y;
	z = 0;
	w = 0;
}


vec4::vec4(const vec3 & v) {

	x = v.x;
	y = v.y;
	z = v.z;
	w = 0;
}

vec4::vec4(const vec3 & v, float w)
{
	x = v.x;
	y = v.y;
	z = v.z;
	this->w = w;
}

vec4::vec4(const Color & color) {

	x = color.r;
	y = color.g;
	z = color.b;
	w = color.a;
}

vec4::vec4(float scalar) {
	this->x = x;
	this->y = x;
	this->z = x;
	this->w = x;
}

vec4::vec4(float x, float y) {
	this->x = x;
	this->y = y;
	this->z = 0;
	this->w = 0;
}

vec4::vec4(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = 0;
}

vec4::vec4(float x, float y, float z, float w) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}


void vec4::set(float scalar) {
	this->x = scalar;
	this->y = scalar;
	this->z = scalar;
	this->w = scalar;
}

void vec4::set(float x, float y, float z, float w) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

void vec4::add(const vec4 & v) {
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;
	this->w += v.w;
}

void vec4::mult(const vec4 & v) {
	this->x *= v.x;
	this->y *= v.y;
	this->z *= v.z;
	this->w *= v.w;
}

void vec4::div(const vec4 & v) {
	this->x /= v.x;
	this->y /= v.y;
	this->z /= v.z;
	this->w /= v.w;
}

void vec4::sub(const vec4 & v) {
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;
	this->w -= v.w;
}

void vec4::set(const Color & color) {
	this->x = color.r;
	this->y = color.g;
	this->z = color.b;
	this->w = color.a;
}

float vec4::norm() const
{
	return x*x + y*y + z*z + w*w; 
}

vec4 vec4::normalize()
{
	float length = this->norm();
	if (length > 0) {
		float f = 1 / sqrt(length);
		x *= f;
		y *= f;
		z *= f;
	}
	return *this;
}

Color vec4::toColor() {

	return Color(Math::clamp(x, 0.f, 1.f), Math::clamp(y, 0.f, 1.f), Math::clamp(z, 0.f, 1.f), Math::clamp(w, 0.f, 1.f));

}

// Operators

bool vec4::operator ==(const vec4 & other) {

	return (x == other.x) && (y == other.y) && (z == other.z) && (w == other.w);

}

bool vec4::operator !=(const vec4 & other) {

	return (x != other.x) || (y != other.y) || (z != other.z) && (w != other.w);

}

// Relational Operators

bool vec4::operator<(const vec4 & other) const {

	return (this->x < other.x) && (this->y < other.y) && (this->z < other.z) && (this->w < other.w);

}
bool vec4::operator<=(const vec4 & other) const {

	return (this->x <= other.x) && (this->y <= other.y) && (this->z <= other.z) && (this->w <= other.w);

}

bool vec4::operator>(const vec4 & other) const {

	return (this->x > other.x) && (this->y > other.y) && (this->z > other.z) && (this->w > other.w);

}

bool vec4::operator>=(const vec4 & other) const {

	return (this->x >= other.x) && (this->y >= other.y) && (this->z >= other.z) && (this->w >= other.w);

}

// Unary Binary

// =

vec4 & vec4::operator = (const vec4 & other) {

	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
	this->w = other.w;
	return *this;

}

vec4 & vec4::operator =(float scalar) {

	this->x = scalar;
	this->y = scalar;
	this->z = scalar;
	this->w = scalar;
	return *this;

}


vec4 & vec4::operator+= (vec4 const & other) {
	this->x += other.x;
	this->y += other.y;
	this->z += other.z;
	this->w += other.w;
	return *this;
}

vec4 & vec4::operator-= (vec4 const & other) {
	this->x -= other.x;
	this->y -= other.y;
	this->z -= other.z;
	this->w -= other.w;
	return *this;
}

vec4 & vec4::operator+= (float other) {
	this->x += other;
	this->y += other;
	this->z += other;
	this->w += other;
	return *this;
}

vec4 & vec4::operator-= (float other) {
	this->x -= other;
	this->y -= other;
	this->z -= other;
	this->w -= other;
	return *this;
}

// *= /=

vec4 & vec4::operator*= (vec4 const & other) {
	this->x *= other.x;
	this->y *= other.y;
	this->z *= other.z;
	this->w *= other.w;
	return *this;
}

vec4 & vec4::operator/= (vec4 const & other) {
	this->x /= other.x;
	this->y /= other.y;
	this->z /= other.z;
	this->w /= other.w;
	return *this;
}

vec4 & vec4::operator*= (float other) {
	this->x *= other;
	this->y *= other;
	this->z *= other;
	this->w *= other;
	return *this;
}

vec4 & vec4::operator/= (float other)
{
	this->x /= other;
	this->y /= other;
	this->z /= other;
	this->w /= other;
	return *this;
}

vec4 operator+ (vec4 const & v1, vec4 const & v2)
{
	return vec4(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w);
}

vec4 operator- (vec4 const & v1, vec4 const & v2)
{
	return vec4(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w);
}

vec4 operator* (vec4 const & v1, vec4 const & v2)
{
	return vec4(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z,  v1.w * v2.w);
}

vec4 operator/ (vec4 const & v1, vec4 const & v2)
{
	return vec4(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.w / v2.w);
}

// unary negate
vec4 operator- (vec4 const & v1)
{
	return vec4(-v1.x, -v1.y, -v1.z, -v1.w);
}

// by scalar

vec4 operator+ (vec4 const & v, float f)
{
	return vec4(v.x + f, v.y + f, v.z + f, v.w + f);
}

vec4 operator- (vec4 const & v, float f)
{
	return vec4(v.x - f, v.y - f, v.z - f, v.w - f);
}

vec4 operator* (vec4 const & v, float f)
{
	return vec4(v.x * f, v.y * f, v.z * f, v.w * f);
}

vec4 operator/ (vec4 const & v, float f)
{
	return vec4(v.x / f, v.y / f, v.z / f, v.w / f);
}