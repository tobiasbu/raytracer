
#include "math/maths.h"

//const static float MachineEpsilon = std::numeric_limits<float>::epsilon() * 0.5;

bool Math::solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1)
{
	float discr = b * b - 4 * a * c;
	if (discr < 0) 
		return false;
	else if (discr == 0) {
		x0 = x1 = -0.5f * b / a;
	}
	else {
		float q = (b > 0) ?
			-0.5f * (b + sqrt(discr)) :
			-0.5f * (b - sqrt(discr));
		x0 = q / a;
		x1 = c / q;
	}
	return true;

	
}

float Math::lerp(const float & from, const float & to, const float & t)
{
	return (1 - t) * from + t * to;
}


float Math::unclampedLerp(float from, float to, float t)
{
	return from + t * (to - from);
}

float Math::blerp(const float &  v00, const float &  v10, const float &  v01, const float &  v11, const float &  tx, const float &  ty)
{
	return lerp(lerp(v00, v10, tx), lerp(v01, v11, tx), ty);
}

int Math::clampint(const int & value, const int & min, const int & max)
{
	return (value > min) ? (value < max) ? value : max : min;
}

const float & Math::clamp(const float &  value, const float &  min, const float &  max) {

	return (value > min) ? (value < max) ? value : max : min;

}

const float & Math::max(const float & a, const float & b)
{
	return (a < b) ? b : a;
		//(left > right) ? right : left;
}

const float & Math::min(const float & a, const float & b)
{
	return !(b < a) ? a : b;
}


float Math::magnitude(const vec3 & v)
{
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

vec3 Math::normalize(const vec3 & v) {

	
	float length = v.norm();
	if (length > 0) {
		float f = 1 / sqrt(length);
		return vec3(v.x * f,v.y * f,v.z * f);
	}
	return vec3(0);

	//return u;

}

float Math::modulo(const float & x)
{
	return x - std::floor(x);
}

float Math::gamma(int n)
{
	
		return (n * MachineEpsilon) / (1 - n * MachineEpsilon);
	
}

float Math::randomRange(const float & from, const float & to)
{

	return from + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (to - from)));
}

// check https://blogs.msdn.microsoft.com/rezanour/2011/08/07/barycentric-coordinates-and-point-in-triangle-tests/
bool Math::inTriangle(const vec3 & v0, const vec3 & v1, const vec3 & v2, const vec3 & point)
{
	vec3 u = v1 - v0; // ba
	vec3 v = v2 - v0; // ca
	vec3 w = point - v0;

	// v
	vec3 vCrossW = vec3::cross(v, w);
	vec3 vCrossU = vec3::cross(v, u);

	// Test sign of r
	if (vec3::dot(vCrossW, vCrossU) < 0)
		return false;

	// u
	vec3 uCrossW = vec3::cross(u, w);
	vec3 uCrossV = vec3::cross(u, v);

	// Test sign of t
	if (vec3::dot(uCrossW, uCrossV) < 0)
		return false;

	// At this point, we know that r and t and both > 0.
	// Therefore, as long as their sum is <= 1, each must be less <= 1
	float denom = uCrossV.magnitude();
	float r = vCrossW.magnitude() / denom;
	float t = uCrossW.magnitude() / denom;


	return (r + t <= 1);

	// normal of plane -> n = ba x ca
	//vec3 n = vec3::cross(u, v);
	//float area2 = n.magnitude(); // area of parallelogram - triangle



	/*float dot00 = dot(v0, v0)
	float dot01 = dot(v0, v1)
	float dot02 = dot(v0, v2)
	float dot11 = dot(v1, v1)
	float dot12 = dot(v1, v2)


	float d = vec3::dot(n, n);

	float y = vec3::dot(vec3::cross(u, w), n) / d;
	float b = vec3::dot(vec3::cross(v, w), n) / d;
	float a = 1 - y - b;

	// Projected point
	vec3 p = (a * v0) + (b * v1) + (y * v2);

	if (a >= 0 && a <= 1
		&& b >= 0 && b <= 1
		&& y >= 0 && y <= 1)
	{
		return true;
	}
	else
		return false;*/
}

vec3 Math::computeNormal(const vec3 & v0, const vec3 & v1, const vec3 & v2)
{
	vec3 normal, U, V;

	U = v0 - v1;//v1 - v0;
	V = v0 - v2;//v2 - v0;

	//cross product
	normal = vec3::cross(U, V).normalized();
	//normal.x = ((U.y * V.z) - (U.z * V.y));
	//normal.y = ((U.z * V.x) - (U.x * V.z));
	//normal.z = ((U.x * V.y) - (U.y * V.x));

	//modulo
	/*float lenght = sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);

	//evitar divisão com zero, normaliza
	if (lenght != 0) {
		normal.x = normal.x / lenght;
		normal.y = normal.y / lenght;
		normal.z = normal.z / lenght;
	}*/

	//if (normal.y < 0) {normal.y *= -1;}
	//if (normal.x < 0) {normal.x *= -1;}
	//if (normal.z < 0) {normal.z *= -1;}

	return normal;
}