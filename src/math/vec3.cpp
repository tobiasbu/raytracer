
#include "math/vec4.h"
#include "math/vec3.h"
#include "math/vec2.h"
#include "render\color.h"

#include <math.h>
#include "math/maths.h"

const vec3 vec3::zero(0.0);


vec3::vec3() 
	:
	x(0), y(0), z(0)
{

}

vec3::vec3(const vec3 & v) {

	x = v.x;
	y = v.y;
	z = v.z;

}

vec3::vec3(const vec2 & v) {

	x = v.x;
	y = v.y;
	z = 0;

}

vec3::vec3(const vec4 & v) 
{
	x = v.x;
	y = v.y;
	z = v.z;
}

vec3::vec3(const Color & color) {

	x = color.r;
	y = color.g;
	z = color.b;

}

vec3::vec3(float scalar) {
	this->x = scalar;
	this->y = scalar;
	this->z = scalar;
}

vec3::vec3(float x, float y) {
	this->x = x;
	this->y = y;
	this->z = 0;
}

vec3::vec3(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

void vec3::set(float scalar) {
	this->x = scalar;
	this->y = scalar;
	this->z = scalar;
}

void vec3::set(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

void vec3::add(const vec3 & v) {
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;
}

void vec3::mult(const vec3 & v) {
	this->x *= v.x;
	this->y *= v.y;
	this->z *= v.z;
}

void vec3::div(const vec3 & v) {
	this->x /= v.x;
	this->y /= v.y;
	this->z /= v.z;
}

void vec3::sub(const vec3 & v) {
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;
}

void vec3::set(const Color & color) {
	this->x = color.r;
	this->y = color.g;
	this->z = color.b;
}

Color vec3::toColor()
{

	return Color(Math::clamp(x, 0.f, 1.f), Math::clamp(y, 0.f, 1.f), Math::clamp(z, 0.f, 1.f), 1);

}

float vec3::norm() const
{


	return x*x + y*y + z*z;

}

float vec3::magnitude() 
{

	return sqrtf(this->norm());

}

float vec3::maxComponent()
{
	return std::fmaxf(x, std::fmaxf(y, z));
}


void vec3::negate() {

	x = -x;
	y = -y;
	z = -z;

}

vec3 vec3::normalized() const 
{

	

	float length = this->norm();
	if (length > 0) {
		float f = 1 / sqrt(length);
		return vec3(x * f, y * f, z * f);
	}
	else
		return *this;

}

void vec3::normalize() {

	float length = this->norm();
	if (length > 0) {
		float f = 1 / sqrt(length);
		x *= f;
		y *= f;
		z *= f;
	}

	/*float magnitude = this->magnitude();
	if (magnitude > 0) {
		x /= magnitude;
		y /= magnitude;
		z /= magnitude;
	}*/

	

}

/*vec3 vec3::normalize(const vec3 & v) {

	vec3 u = v;

	float magnitude = u.magnitude();
	if (magnitude > 0) {
		return vec3(u.x / magnitude, u.y / magnitude, u.z / magnitude);
	}
	else
		u = vec3(0);
	

	//float length = u.norm();
	//if (length > 0) {
	//	float f = 1 / sqrt(length);
	//	u.x *= f;
	//	u.y *= f;
	//	u.z *= f;
	//}

	return u;

}*/



vec3 vec3::negative() {

	return vec3(-x, -y, -z);

}


vec3 vec3::abs(const vec3 & v) 
{
	return vec3(std::fabsf(v.x), std::fabsf(v.y), std::fabsf(v.z));
}

float vec3::dot(const vec3 & v1, const vec3 & v2) 
{

	// return x * v.x + y * v.y + z * v.z;
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;

}

vec3 vec3::cross(const vec3 & v1, const vec3 & v2) 
{

	// return Vec3<T>(y * v.z - z * v.y,
	//				  z * v.x - x * v.z,
	//				  x * v.y - y * v.x);
	return vec3(v1.y*v2.z - v1.z*v2.y,
				v1.z*v2.x - v1.x*v2.z,
				v1.x*v2.y - v1.y*v2.x);

}

float vec3::distance(const vec3 & v1, const vec3 & v2) 
{

	return sqrt(pow(v1.x-v2.x,2) + pow(v1.y-v2.y,2) + pow(v1.z*v2.z,2));

}

float vec3::angle(const vec3 & v1, const vec3 & v2)
{
	float r = dot(v1, v2);
	r /= (Math::magnitude(v1) * Math::magnitude(v2));
	r = acosf(r);
	return r;
}

vec3 vec3::reflect(const vec3 & i, const vec3 & n) 
{

	return i - 2 * vec3::dot(i,n) * n;

}

vec3 vec3::refract(const vec3 & i, const vec3 & n, const float & ior)
{
	float cosi = Math::clamp(-1, 1, vec3::dot(i,n));
	float etai = 1, etat = ior;
	vec3 nn = n;

	if (cosi < 0) 
	{ 
		cosi = -cosi; 
	}
	else 
	{ 
		std::swap(etai, etat); nn = -n;
	}

	float eta = etai / etat;
	float k = 1 - eta * eta * (1 - cosi * cosi);
	return k < 0 ? 0 : eta * i + (eta * cosi - sqrtf(k)) * n;
}

vec3 vec3::lerp(const vec3 & from, const vec3 & to, float t) {

	return vec3(
		Math::lerp(from.x, to.x, t),
		Math::lerp(from.y, to.y, t),
		Math::lerp(from.z, to.z, t)
		);

}

// Operators

bool vec3::operator ==(const vec3 & other) {

	return (x == other.x) && (y == other.y) && (z == other.z);

}

bool vec3::operator !=(const vec3 & other) {

	return (x != other.x) || (y != other.y) || (z != other.z);

}

// Relational Operators

bool vec3::operator<(const vec3 & other) const {

	return (this->x < other.x) && (this->y < other.y) && (this->z < other.z);

}
bool vec3::operator<=(const vec3 & other) const {

	return (this->x <= other.x) && (this->y <= other.y) && (this->z <= other.z);

}

bool vec3::operator>(const vec3 & other) const {

	return (this->x > other.x) && (this->y > other.y) && (this->z > other.z);

}

bool vec3::operator>=(const vec3 & other) const {

	return (this->x >= other.x) && (this->y >= other.y) && (this->z >= other.z);

}

// Unary Binary

// =

vec3 & vec3::operator = (const vec3 & other) {

	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
	return *this;

}

vec3 & vec3::operator =(float scalar) {

	this->x = scalar;
	this->y = scalar;
	this->z = scalar;

	return *this;

}

// + -


/*vec3 vec3::operator+ (float other) {

	vec3 v(x + other, y + other, z + other);
	return v;
}

/*vec3 vec3::operator- (vec3 const & other) {

	vec3 v(x - other.x, y - other.y, y - other.z);
	return v;

}


vec3 vec3::operator- (float other) {

	vec3 v(x - other, y - other, z - other);
	return v;

}

// * /


vec3 vec3::operator* (float other) {
	vec3 v(x * other, y * other, z * other);
	return v;
}

vec3 vec3::operator/ (float other) {
	vec3 v(x / other, y / other, z / other);
	return v;
}*/

// += -=

vec3 & vec3::operator+= (vec3 const & other) {
	this->x += other.x;
	this->y += other.y;
	this->z += other.z;
	return *this;
}

vec3 & vec3::operator-= (vec3 const & other) {
	this->x -= other.x;
	this->y -= other.y;
	this->z -= other.z;
	return *this;
}

vec3 & vec3::operator+= (float other) {
	this->x += other;
	this->y += other;
	this->z += other;
	return *this;
}

vec3 & vec3::operator-= (float other) {
	this->x -= other;
	this->y -= other;
	this->z -= other;
	return *this;
}

// *= /=

vec3 & vec3::operator*= (vec3 const & other) {
	this->x *= other.x;
	this->y *= other.y;
	this->z *= other.z;
	return *this;
}

vec3 & vec3::operator/= (vec3 const & other) {
	this->x /= other.x;
	this->y /= other.y;
	this->z /= other.z;
	return *this;
}

vec3 & vec3::operator*= (float other) {
	this->x *= other;
	this->y *= other;
	this->z *= other;
	return *this;
}

vec3 & vec3::operator/= (float other) 
{
	this->x /= other;
	this->y /= other;
	this->z /= other;
	return *this;
}

vec3 operator+ (vec3 const & v1, vec3 const & v2)
{
	return vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

vec3 operator- (vec3 const & v1, vec3 const & v2) 
{
	return vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

vec3 operator* (vec3 const & v1, vec3 const & v2)
{
	return vec3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

vec3 operator/ (vec3 const & v1, vec3 const & v2) 
{
	return vec3(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
}

// unary negate
vec3 operator- (vec3 const & v1) 
{
	return vec3(-v1.x, -v1.y, -v1.z);
}

// by scalar

vec3 operator+ (vec3 const & v, float f) 
{
	return vec3(v.x + f, v.y + f, v.z + f);
}

vec3 operator- (vec3 const & v, float f)
{
	return vec3(v.x - f, v.y - f, v.z - f);
}

vec3 operator* (vec3 const & v, float f) 
{
	return vec3(v.x * f, v.y * f, v.z * f);
}

vec3 operator* (float f, vec3 const & v) 
{
	return vec3(f * v.x, f * v.y , f * v.z);
}

vec3 operator/ (vec3 const & v, float f) 
{
	return vec3(v.x / f, v.y / f, v.z / f);
}