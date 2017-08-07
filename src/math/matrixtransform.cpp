
#include "math/matrixtransform.h"
#include "math/maths.h"

// we are using here ROW MAJOR
// the foll

/* ROW MAJOR



float row_major_t[16] =    
{
1, 0, 0, 0,
0, 1, 0, 0,
0, 0, 1, 0,
x, y, z, 1
};

float column_major_t[16] = {
1, 0, 0, x,
0, 1, 0, y,
0, 0, 1, z,
0, 0, 0, 1};

collum | row
[0][1]

row major order:
we stored like this:
0  1  2  3
4  5  6  7
8  9  10 11
12 13 14 15

*/

mat4 MatrixTransform::translate(const mat4& m, const vec3& v) {

	mat4 m_t = mat4::identity;
	m_t.m[12] = v[0];
	m_t.m[13] = v[1];
	m_t.m[14] = v[2];
	return m_t * m;

}

// rotate around x axis by an angle in degrees
mat4 MatrixTransform::rotate_x_deg(const mat4& m, float deg) {

	// 1 0  0 0
	// 0 c -s 0
	// 0 s  c 0
	// 0 0  0  1


	// convert to radians
	float rad = deg * DEG_TO_RAD;
	mat4 m_r = mat4::identity;
	m_r.m[5] = cos(rad);
	m_r.m[6] = -sin(rad); // 6 = sin
	m_r.m[9] = sin(rad); // 9 = -sin
	m_r.m[10] = cos(rad);
	return m_r * m;

}

// rotate around y axis by an angle in degrees
mat4 MatrixTransform::rotate_y_deg(const mat4& m, float deg) {

	// convert to radians
	float rad = deg * DEG_TO_RAD;
	mat4 m_r = mat4::identity;

	// c  0  s 0
	// 0  1  0 0
	// -s 0  c 0
	// 0  0  0 1

	// collum

	m_r.m[0] = cos(rad);
	m_r.m[2] = sin(rad); // 	m_r.m[2] = -sin(rad);
	m_r.m[8] = -sin(rad);
	m_r.m[10] = cos(rad);
	return m_r * m;

}

// rotate around z axis by an angle in degrees
mat4 MatrixTransform::rotate_z_deg(const mat4& m, float deg) {


	// c -s  0 0
	// s  c  0 0
	// 0  0  1 0
	// 0  0  0 1

	// convert to radians
	float rad = deg * DEG_TO_RAD;
	mat4 m_r = mat4::identity;
	m_r.m[0] = cos(rad);
	m_r.m[1] = -sin(rad); // m_r.m[1] = sin(rad);
	m_r.m[4] = sin(rad);
	
	m_r.m[5] = cos(rad);
	return m_r * m;

}

// scale a matrix by [x, y, z]
mat4 MatrixTransform::scale(const mat4& m, const vec3& v) {

	mat4 a = mat4::identity;
	a.m[0] = v.x;
	a.m[5] = v.y;
	a.m[10] = v.z;
	return a * m;

}

// multiply view matrix by directional vector
vec3 MatrixTransform::multiplyPoint(const mat4 & m, const vec3 & v) {

	vec3 u;

	// x * m[0] + y * m[1] + z * m[2] (fourth columm is ignored)
	// a = src[0] * x[0][0] + src[1] * x[1][0] + src[2] * x[2][0];
	u.x = v.x * m.m[0] + v.y * m.m[4] + v.z * m.m[8];

	// x * m[4] + y * m[5] + z * m[6]
	// b = src[0] * x[0][1] + src[1] * x[1][1] + src[2] * x[2][1];
	u.y = v.x * m.m[1] + v.y * m.m[5] + v.z * m.m[9];

	// x * m[8] + y * m[9] + z * m[10]
	// src[0] * x[0][2] + src[1] * x[1][2] + src[2] * x[2][2];
	u.z = v.x * m.m[2] + v.y * m.m[6] + v.z * m.m[10];

	return u;

}

vec3 MatrixTransform::multiplyVec(const mat4 & a, const vec3 & v) {

	vec3 u;
	float w;

	// a = src[0] * x[0][0] + src[1] * x[1][0] + src[2] * x[2][0] + x[3][0]; 
	u.x = v[0] * a.m[0] + v[1] * a.m[4] + v[2] * a.m[8] + a.m[12];

	// b = src[0] * x[0][1] + src[1] * x[1][1] + src[2] * x[2][1] + x[3][1];
	u.y = v[0] * a.m[1] + v[1] * a.m[5] + v[2] * a.m[9] + a.m[13];

	//  c = src[0] * x[0][2] + src[1] * x[1][2] + src[2] * x[2][2] + x[3][2]; 
	u.z = v[0] * a.m[2] + v[1] * a.m[6] + v[2] * a.m[10] + a.m[14];

	// w = src[0] * x[0][3] + src[1] * x[1][3] + src[2] * x[2][3] + x[3][3];
	w = v[0] * a.m[3] + v[1] * a.m[7] + v[2] * a.m[11] + a.m[15];

	u.x /= w;
	u.y /= w;
	u.z /= w;

	return u;
}

mat4 MatrixTransform::perspective(float fovy, float aspect, float near, float far) {

	float fov_rad = fovy * DEG_TO_RAD;
	float range = tan(fov_rad / 2.0f) * near;
	float sx = (2.0f * near) / (range * aspect + range * aspect);
	float sy = near / range;
	float sz = -(far + near) / (far - near);
	float pz = -(2.0f * far * near) / (far - near);
	mat4 m = mat4::zero; // make sure bottom-right corner is zero
	m.m[0] = sx; // 1 / aspect * tan(fov / 2)
	m.m[5] = sy; // 1 / tan(fov / 2)
	m.m[10] = sz; // (far + near) / (far - near)

	
	m.m[11] = pz; // -  (2 * far * near) / (far - near) 
	m.m[14] = -1.0;
	
	//m.m[11] = -1.0f; // hand

	return m;

}

mat4 MatrixTransform::ortho(float left, float right, float bottom, float top, float near, float far)
{

	mat4 mat;
	mat[0] = 2 / (right - left);
	mat[5] = 2 / (top - bottom);
	mat[10] = -2 / (far - near);
	mat[12] = -(right + left) / (right - left);
	mat[13] = -(top + bottom) / (top - bottom);
	mat[14] = -(far + near) / (far - near);
	return mat;
}

/*

//OLD METHOD
mat4 MatrixTransform::lookAt2(const vec3 & cam_pos, const vec3 & targ_pos, const vec3& up) {
	// inverse translation
	mat4 p = mat4::identity;
	p = MatrixTransform::translate(p, vec3(-cam_pos[0], -cam_pos[1], -cam_pos[2]));
	// distance vector
	vec3 d = targ_pos - cam_pos;
	// forward vector
	vec3 f = d.normalized();
	// right vector
	vec3 r = vec3::cross(f, up).normalized();
	// real up vector
	vec3 u = vec3::cross(r, f).normalized();
	mat4 ori = mat4::identity;
	ori.m[0] = r[0];
	ori.m[4] = r[1];
	ori.m[8] = r[2];
	ori.m[1] = u[0];
	ori.m[5] = u[1];
	ori.m[9] = u[2];
	ori.m[2] = -f[0];
	ori.m[6] = -f[1];
	ori.m[10] = -f[2];

	// 1 1 0 0
	// 1 1 1 0
	// 1

	return ori * p;//p * ori;
} */


void MatrixTransform::setDirection(mat4 & a, const vec3 & dir) {

	a.m[8] = dir.x;
	a.m[9] = dir.y;
	a.m[10] = dir.z;

}

void MatrixTransform::setPosition(mat4 & a, const vec3 & pos) {

	a.m[12] = pos.x;
	a.m[13] = pos.y;
	a.m[14] = pos.z;

}

void MatrixTransform::setUp(mat4 & a, const vec3 & dir, const vec3 & up) {

	//vec3 tmp = Math::normalize(up);
	vec3 right = vec3::cross(up, dir).normalized(); // right
	vec3 up_final = vec3::cross(dir, right).normalized(); // up

	a.m[0] = right.x;
	a.m[1] = right.y;
	a.m[2] = right.z;

	a.m[4] = up_final.x;
	a.m[5] = up_final.y;
	a.m[6] = up_final.z;

}

mat4 MatrixTransform::lookAt(const vec3 & eye, const vec3 & center, const vec3 & up) {


	// final matrix
	mat4 mat = mat4::identity;

	// the axis: z = forward; y = up; x = right;
	vec3 x, y, z;

	
	//mat = MatrixTransform::translate(mat, vec3(-eye.x, -eye.y, -eye.z));

	// distance vector
	z = Math::normalize(center - eye);

	/*
	(tmp = up)
	Vec3f forward = normalize(from - to);
	Vec3f right = crossProduct(normalize(tmp), forward);
	Vec3f up = crossProduct(forward, right);*/

	// create coordinate system
	//z = vec3::normalize(eye - center);
	//z.normalized(); // forward

	// set right axis
	x = vec3::cross(up.normalized(),z).normalized(); // right

	// set up axis
	y = vec3::cross(z,x).normalized(); // up

	// The length of the cross product is equal to the area of the parallelogram, 
	// which is < 1.0 for non-perpendicular unit-length vectors; so normalize X, Y here: 
	//x.normalized();
	//y.normalized();

	// matrix 
	// [row, colum] -  [i,j] 
	// y x
	// [0,0] [0,1] [0,2] [0,3]
	// [1,0] [1,1] [1,2] [1,3]
	// [2,0] [2,1] [2,2] [2,3]
	// [3,0] [3,1] [3,2] [3,3]

	// 0 1 2 3
	// 4 5 6 7
	// 8 9 10 11
	// 12 13 14 15

	mat.m[0] = x.x;
	mat.m[1] = x.y;
	mat.m[2] = x.z;

	mat.m[4] = y.x;
	mat.m[5] = y.y;
	mat.m[6] = y.z;

	mat.m[8] = z.x;
	mat.m[9] = z.y;
	mat.m[10] = z.z;

	mat.m[12] = eye.x;
	mat.m[13] = eye.y;
	mat.m[14] = eye.z;

	// right

	/*mat.m[0] = x.x;
	mat.m[1] = x.y;
	mat.m[2] = x.z;
	//3

	// up

	mat.m[4] = y.x;
	mat.m[5] = y.y;
	mat.m[6] = y.z;
	//7

	// forward

	mat.m[8] = z.x;
	mat.m[9] = z.y;
	mat.m[10] = z.z;
	//11

	// position
	mat.m[12] = eye.x;
	mat.m[13] = eye.y;
	mat.m[14] = eye.z;*/
	
	return mat;

}