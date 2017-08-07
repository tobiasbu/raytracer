/******************************************************************************\
| OpenGL 4 Example Code.                                                       |
| Accompanies written series "Anton's OpenGL 4 Tutorials"                      |
| Email: anton at antongerdelan dot net                                        |
| First version 27 Jan 2014                                                    |
| Copyright Dr Anton Gerdelan, Trinity College Dublin, Ireland.                |
| See individual libraries' separate legal notices                             |
|******************************************************************************|
| Commonly-used maths structures and functions                                 |
| Simple-as-possible. No templates. Not optimised.                             |
| Structs vec3, mat4, versor. just hold arrays of floats called "v","m","q",   |
| respectively. So, for example, to get values from a mat4 do: my_mat.m        |
| A versor is the proper name for a unit quaternion.                           |
\******************************************************************************/
#include "math\maths_funcs.h"
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>

/*--------------------------------CONSTRUCTORS--------------------------------*/


/*mat4::mat4() {}

// note: entered in COLUMNS 
mat4::mat4(float a, float b, float c, float d,
	float e, float f, float g, float h,
	float i, float j, float k, float l,
	float mm, float n, float o, float p) {
	m[0] = a;
	m[1] = b;
	m[2] = c;
	m[3] = d;
	m[4] = e;
	m[5] = f;
	m[6] = g;
	m[7] = h;
	m[8] = i;
	m[9] = j;
	m[10] = k;
	m[11] = l;
	m[12] = mm;
	m[13] = n;
	m[14] = o;
	m[15] = p;
}*/

/*-----------------------------PRINT FUNCTIONS--------------------------------*/
void print(const vec2& v) {
	printf("[%.2f, %.2f]\n", v[0], v[1]);
}

void print(const vec3& v) {
	printf("[%.2f, %.2f, %.2f]\n", v[0], v[1], v[2]);
}

void print(const vec4& v) {
	printf("[%.2f, %.2f, %.2f, %.2f]\n", v[0], v[1], v[2], v[3]);
}

void print(const mat3& m) {
	printf("\n");
	printf("[%.2f][%.2f][%.2f]\n", m.m[0], m.m[3], m.m[6]);
	printf("[%.2f][%.2f][%.2f]\n", m.m[1], m.m[4], m.m[7]);
	printf("[%.2f][%.2f][%.2f]\n", m.m[2], m.m[5], m.m[8]);
}

void print(const mat4& m) {
	printf("\n");
	printf("[%.2f][%.2f][%.2f][%.2f]\n", m.m[0], m.m[4], m.m[8], m.m[12]);
	printf("[%.2f][%.2f][%.2f][%.2f]\n", m.m[1], m.m[5], m.m[9], m.m[13]);
	printf("[%.2f][%.2f][%.2f][%.2f]\n", m.m[2], m.m[6], m.m[10], m.m[14]);
	printf("[%.2f][%.2f][%.2f][%.2f]\n", m.m[3], m.m[7], m.m[11], m.m[15]);
}

/*------------------------------VECTOR FUNCTIONS------------------------------*/
float length(const vec3& v) {
	return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

// squared length
float length2(const vec3& v) {
	return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
}

// note: proper spelling (hehe)
vec3 normalise(const vec3& v) {
	vec3 vb;
	float l = length(v);
	if (0.0f == l) {
		return vec3(0.0f, 0.0f, 0.0f);
	}
	vb[0] = v[0] / l;
	vb[1] = v[1] / l;
	vb[2] = v[2] / l;
	return vb;
}

/*vec3 vec3::operator+ (const vec3& rhs) {
	vec3 vc;
	vc[0] = v[0] + rhs[0];
	vc[1] = v[1] + rhs[1];
	vc[2] = v[2] + rhs[2];
	return vc;
}

vec3& vec3::operator+= (const vec3& rhs) {
	v[0] += rhs[0];
	v[1] += rhs[1];
	v[2] += rhs[2];
	return *this; // return self
}

vec3 vec3::operator- (const vec3& rhs) {
	vec3 vc;
	vc[0] = v[0] - rhs[0];
	vc[1] = v[1] - rhs[1];
	vc[2] = v[2] - rhs[2];
	return vc;
}


vec3& vec3::operator-= (vec3 const & rhs) {
	v[0] -= rhs[0];
	v[1] -= rhs[1];
	v[2] -= rhs[2];
	return *this;
}



vec3 vec3::operator+ (float rhs) {
	vec3 vc;
	vc[0] = v[0] + rhs;
	vc[1] = v[1] + rhs;
	vc[2] = v[2] + rhs;
	return vc;
}

vec3 vec3::operator- (float rhs) {
	vec3 vc;
	vc[0] = v[0] - rhs;
	vc[1] = v[1] - rhs;
	vc[2] = v[2] - rhs;
	return vc;
}

vec3 vec3::operator* (float rhs) {
	vec3 vc;
	vc[0] = v[0] * rhs;
	vc[1] = v[1] * rhs;
	vc[2] = v[2] * rhs;
	return vc;
}

vec3 vec3::operator/ (float rhs) {
	vec3 vc;
	vc[0] = v[0] / rhs;
	vc[1] = v[1] / rhs;
	vc[2] = v[2] / rhs;
	return vc;
}

vec3& vec3::operator*= (float rhs) {
	v[0] = v[0] * rhs;
	v[1] = v[1] * rhs;
	v[2] = v[2] * rhs;
	return *this;
}

vec3& vec3::operator= (const vec3& rhs) {
	v[0] = rhs[0];
	v[1] = rhs[1];
	v[2] = rhs[2];
	return *this;
}*/


float dot(const vec3& a, const vec3& b) {
	return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

vec3 cross(const vec3& a, const vec3& b) {
	float x = a[1] * b[2] - a[2] * b[1];
	float y = a[2] * b[0] - a[0] * b[2];
	float z = a[0] * b[1] - a[1] * b[0];
	return vec3(x, y, z);
}

float get_squared_dist(vec3 from, vec3 to) {
	float x = (to[0] - from[0]) * (to[0] - from[0]);
	float y = (to[1] - from[1]) * (to[1] - from[1]);
	float z = (to[2] - from[2]) * (to[2] - from[2]);
	return x + y + z;
}

/* converts an un-normalised direction into a heading in degrees
NB i suspect that the z is backwards here but i've used in in
several places like this. d'oh! */
float direction_to_heading(vec3 d) {
	return atan2f(-d[0], -d[2]) * ONE_RAD_IN_DEG;
}

vec3 heading_to_direction(float degrees) {
	float rad = degrees * ONE_DEG_IN_RAD;
	return vec3(-sinf(rad), 0.0f, -cosf(rad));
}

/*-----------------------------MATRIX FUNCTIONS-------------------------------*/
mat3 zero_mat3() {
	return mat3(
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f
		);
}

mat3 identity_mat3() {
	return mat3(
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
		);
}



/* mat4 array layout
0  4  8 12
1  5  9 13
2  6 10 14
3  7 11 15
*/









/*----------------------------HAMILTON IN DA HOUSE!---------------------------*/
versor::versor() { }

versor versor::operator/ (float rhs) {
	versor result;
	result.q[0] = q[0] / rhs;
	result.q[1] = q[1] / rhs;
	result.q[2] = q[2] / rhs;
	result.q[3] = q[3] / rhs;
	return result;
}

versor versor::operator* (float rhs) {
	versor result;
	result.q[0] = q[0] * rhs;
	result.q[1] = q[1] * rhs;
	result.q[2] = q[2] * rhs;
	result.q[3] = q[3] * rhs;
	return result;
}

void print(const versor& q) {
	printf("[%.2f ,%.2f, %.2f, %.2f]\n", q.q[0], q.q[1], q.q[2], q.q[3]);
}

versor versor::operator* (const versor& rhs) {
	versor result;
	result.q[0] = rhs.q[0] * q[0] - rhs.q[1] * q[1] -
		rhs.q[2] * q[2] - rhs.q[3] * q[3];
	result.q[1] = rhs.q[0] * q[1] + rhs.q[1] * q[0] -
		rhs.q[2] * q[3] + rhs.q[3] * q[2];
	result.q[2] = rhs.q[0] * q[2] + rhs.q[1] * q[3] +
		rhs.q[2] * q[0] - rhs.q[3] * q[1];
	result.q[3] = rhs.q[0] * q[3] - rhs.q[1] * q[2] +
		rhs.q[2] * q[1] + rhs.q[3] * q[0];
	// re-normalise in case of mangling
	return normalise(result);
}

versor versor::operator+ (const versor& rhs) {
	versor result;
	result.q[0] = rhs.q[0] + q[0];
	result.q[1] = rhs.q[1] + q[1];
	result.q[2] = rhs.q[2] + q[2];
	result.q[3] = rhs.q[3] + q[3];
	// re-normalise in case of mangling
	return normalise(result);
}

versor quat_from_axis_rad(float radians, float x, float y, float z) {
	versor result;
	result.q[0] = cos(radians / 2.0);
	result.q[1] = sin(radians / 2.0) * x;
	result.q[2] = sin(radians / 2.0) * y;
	result.q[3] = sin(radians / 2.0) * z;
	return result;
}

versor quat_from_axis_deg(float degrees, float x, float y, float z) {
	return quat_from_axis_rad(ONE_DEG_IN_RAD * degrees, x, y, z);
}

mat4 quat_to_mat4(const versor& q) {
	float w = q.q[0];
	float x = q.q[1];
	float y = q.q[2];
	float z = q.q[3];
	return mat4(
		1.0f - 2.0f * y * y - 2.0f * z * z,
		2.0f * x * y + 2.0f * w * z,
		2.0f * x * z - 2.0f * w * y,
		0.0f,
		2.0f * x * y - 2.0f * w * z,
		1.0f - 2.0f * x * x - 2.0f * z * z,
		2.0f * y * z + 2.0f * w * x,
		0.0f,
		2.0f * x * z + 2.0f * w * y,
		2.0f * y * z - 2.0f * w * x,
		1.0f - 2.0f * x * x - 2.0f * y * y,
		0.0f,
		0.0f,
		0.0f,
		0.0f,
		1.0f
		);
}

versor normalise(versor& q) {
	// norm(q) = q / magnitude (q)
	// magnitude (q) = sqrt (w*w + x*x...)
	// only compute sqrt if interior sum != 1.0
	float sum =
		q.q[0] * q.q[0] + q.q[1] * q.q[1] +
		q.q[2] * q.q[2] + q.q[3] * q.q[3];
	// NB: floats have min 6 digits of precision
	const float thresh = 0.0001f;
	if (fabs(1.0f - sum) < thresh) {
		return q;
	}
	float mag = sqrt(sum);
	return q / mag;
}

float dot(const versor& q, const versor& r) {
	return q.q[0] * r.q[0] + q.q[1] * r.q[1] + q.q[2] * r.q[2] + q.q[3] * r.q[3];
}

versor slerp(versor& q, versor& r, float t) {
	// angle between q0-q1
	float cos_half_theta = dot(q, r);
	// as found here http://stackoverflow.com/questions/2886606/flipping-issue-when-interpolating-rotations-using-quaternions
	// if dot product is negative then one quaternion should be negated, to make
	// it take the short way around, rather than the long way
	// yeah! and furthermore Susan, I had to recalculate the d.p. after this
	if (cos_half_theta < 0.0f) {
		for (int i = 0; i < 4; i++) {
			q.q[i] *= -1.0f;
		}
		cos_half_theta = dot(q, r);
	}
	// if qa=qb or qa=-qb then theta = 0 and we can return qa
	if (fabs(cos_half_theta) >= 1.0f) {
		return q;
	}
	// Calculate temporary values
	float sin_half_theta = sqrt(1.0f - cos_half_theta * cos_half_theta);
	// if theta = 180 degrees then result is not fully defined
	// we could rotate around any axis normal to qa or qb
	versor result;
	if (fabs(sin_half_theta) < 0.001f) {
		for (int i = 0; i < 4; i++) {
			result.q[i] = (1.0f - t) * q.q[i] + t * r.q[i];
		}
		return result;
	}
	float half_theta = acos(cos_half_theta);
	float a = sin((1.0f - t) * half_theta) / sin_half_theta;
	float b = sin(t * half_theta) / sin_half_theta;
	for (int i = 0; i < 4; i++) {
		result.q[i] = q.q[i] * a + r.q[i] * b;
	}
	return result;
}