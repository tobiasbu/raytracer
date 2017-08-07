
#include "math/mat4.h"

const mat4 mat4::zero(0.0);
const mat4 mat4::identity(1.0);
//const mat4 mat4::one(1.0,1.0,1.0,1.0);


mat4::mat4() {

	set(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

}

mat4::mat4(float a) {

	/*
	1 0 0 0
	0 1 0 0
	0 0 1 0
	0 0 0 1
	*/

	m[0] = a;
	m[1] = 0;
	m[2] = 0;
	m[3] = 0;

	m[4] = 0;
	m[5] = a;
	m[6] = 0;
	m[7] = 0;

	m[8] = 0;
	m[9] = 0;
	m[10] = a;
	m[11] = 0;

	m[12] = 0;
	m[13] = 0;
	m[14] = 0;
	m[15] = a;

}

mat4::mat4(float a, float b, float c, float d,
	float e, float f, float g, float h,
	float i, float j, float k, float l,
	float mm, float n, float o, float p)
{
	set(a, b, c, d,
		e, f, g, h,
		i, j, k, l,
		mm, n, o, p);

	/*m[0] = a;
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
	m[15] = p;*/
}

void mat4::set(float a, float b, float c, float d,
	float e, float f, float g, float h,
	float i, float j, float k, float l,
	float mm, float n, float o, float p) 
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
	m[9] = j;
	m[10] = k;
	m[11] = l;

	m[12] = mm;
	m[13] = n;
	m[14] = o;
	m[15] = p;
}

/*mat4 mat4::zero() {
	return mat4(
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f
		);
}

mat4 mat4::identity() {
	return mat4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		);
}*/



mat4  mat4::transposed() {

	return mat4::transpose(*this);

}

mat4 mat4::multiply(const mat4 & a, const mat4 & b) {

#if 0 
	for (uint8_t i = 0; i < 4; ++i) {
		for (uint8_t j = 0; j < 4; ++j) {
			c[i][j] = a[i][0] * b[0][j] + a[i][1] * b[1][j] +
				a[i][2] * b[2][j] + a[i][3] * b[3][j];
		}
	}
#else 
	// A restric qualified pointer (or reference) is basically a promise
	// to the compiler that for the scope of the pointer, the target of the
	// pointer will only be accessed through that pointer (and pointers
	// copied from it.
	const float * __restrict ap = &a.m[0];
	const float * __restrict bp = &b.m[0];

	mat4 c;

	float * __restrict cp = &c.m[0];

	float a0, a1, a2, a3;

	a0 = ap[0];
	a1 = ap[1];
	a2 = ap[2];
	a3 = ap[3];

	cp[0] = a0 * bp[0] + a1 * bp[4] + a2 * bp[8] + a3 * bp[12];
	cp[1] = a0 * bp[1] + a1 * bp[5] + a2 * bp[9] + a3 * bp[13];
	cp[2] = a0 * bp[2] + a1 * bp[6] + a2 * bp[10] + a3 * bp[14];
	cp[3] = a0 * bp[3] + a1 * bp[7] + a2 * bp[11] + a3 * bp[15];

	a0 = ap[4];
	a1 = ap[5];
	a2 = ap[6];
	a3 = ap[7];

	cp[4] = a0 * bp[0] + a1 * bp[4] + a2 * bp[8] + a3 * bp[12];
	cp[5] = a0 * bp[1] + a1 * bp[5] + a2 * bp[9] + a3 * bp[13];
	cp[6] = a0 * bp[2] + a1 * bp[6] + a2 * bp[10] + a3 * bp[14];
	cp[7] = a0 * bp[3] + a1 * bp[7] + a2 * bp[11] + a3 * bp[15];

	a0 = ap[8];
	a1 = ap[9];
	a2 = ap[10];
	a3 = ap[11];

	cp[8] = a0 * bp[0] + a1 * bp[4] + a2 * bp[8] + a3 * bp[12];
	cp[9] = a0 * bp[1] + a1 * bp[5] + a2 * bp[9] + a3 * bp[13];
	cp[10] = a0 * bp[2] + a1 * bp[6] + a2 * bp[10] + a3 * bp[14];
	cp[11] = a0 * bp[3] + a1 * bp[7] + a2 * bp[11] + a3 * bp[15];

	a0 = ap[12];
	a1 = ap[13];
	a2 = ap[14];
	a3 = ap[15];

	cp[12] = a0 * bp[0] + a1 * bp[4] + a2 * bp[8] + a3 * bp[12];
	cp[13] = a0 * bp[1] + a1 * bp[5] + a2 * bp[9] + a3 * bp[13];
	cp[14] = a0 * bp[2] + a1 * bp[6] + a2 * bp[10] + a3 * bp[14];
	cp[15] = a0 * bp[3] + a1 * bp[7] + a2 * bp[11] + a3 * bp[15];

	return c;

#endif 
}



// returns a scalar value with the determinant for a 4x4 matrix
// see http://www.euclideanspace.com/maths/algebra/matrix/functions/determinant/fourD/index.htm
float mat4::determinant(const mat4& a) {
	
	// m03 * m12 * m21 * m30 - 
	// 3 * 6 * 9 * 12 -
	// m02 * m13 * m21 * m30 -
	// 2 * 7 * 9 * 12 -
	// m03 * m11 * m22 * m30 + 
	// 3 * 5 * 10 * 12 +
	// m01 * m13 * m22 * m30 +
	// 1 * 7 * 10 * 12
	// m02 * m11 * m23 * m30 - 
	// 2 * 5 * 11 * 12
	// m01 * m12 * m23 * m30 - 
	// 1 * 6 * 11 * 12
	// m03 * m12 * m20 * m31 + 
	// 3 * 6 * 8 * 13
	// m02 * m13 * m20 * m31 +
	// 2 * 7 * 8 * 13 
	// m03 * m10 * m22 * m31 - 
	// 3 * 4 * 10 * 13
	// m00 * m13 * m22 * m31 - 
	// 0 * 7 * 10 * 13
	// m02 * m10 * m23 * m31 + 
	// 2 * 4 * 11 * 13
	// m00 * m12 * m23 * m31 +
	// 0 * 6 * 11 * 13
	// m03 * m11 * m20 * m32 -
	// 3 * 5 * 8 * 14
	// m01 * m13 * m20 * m32 -
	// 1 * 7 * 8 * 14
	// m03 * m10 * m21 * m32 + 
	// 3 * 4 * 9 * 14
	// m00 * m13 * m21 * m32 +
	// 0 * 7 * 9 * 14 
	// m01 * m10 * m23 * m32 - 
	// 1 * 4 * 11 * 14
	// m00 * m11 * m23 * m32 - 
	// 0 * 5 * 11 * 14
	// m02 * m11 * m20 * m33 + 
	// 2 * 5 * 8 * 15
	// m01 * m12 * m20 * m33 +
	// 1 * 6 * 8 * 15
	// m02 * m10 * m21 * m33 - 
	// 2 * 4 * 9 * 15
	// m00 * m12 * m21 * m33 - 
	// 0 * 6 * 9 * 15 -
	// m01 * m10 * m22 * m33 + 
	// 1 * 4 * 10 * 15
	// m00 * m11 * m22 * m33;
	// 0 * 5 * 10 * 15

	return
		// m03 * m12 * m21 * m30 - 
		a.m[3] * a.m[6] * a.m[9] * a.m[12] -
		// m02 * m13 * m21 * m30 -
		a.m[2] * a.m[7] * a.m[9] * a.m[12] -
		// m03 * m11 * m22 * m30 + 
		a.m[3] * a.m[5] * a.m[10] * a.m[12] +
		// m01 * m13 * m22 * m30 +
		a.m[1] * a.m[7] * a.m[10] * a.m[12] +
		// m02 * m11 * m23 * m30 - 
		a.m[2] * a.m[5] * a.m[11] * a.m[12] -
		// m01 * m12 * m23 * m30 - 
		a.m[1] * a.m[6] * a.m[11] * a.m[12] -
		// m03 * m12 * m20 * m31 + 
		a.m[3] * a.m[6] * a.m[8] * a.m[13] +
		// m02 * m13 * m20 * m31 +
		a.m[2] * a.m[7] * a.m[8] * a.m[13] +
		// m03 * m10 * m22 * m31 - 
		a.m[3] * a.m[4] * a.m[10] * a.m[13] -
		// m00 * m13 * m22 * m31 - 
		a.m[0] * a.m[7] * a.m[10] * a.m[13] -
		// m02 * m10 * m23 * m31 + 
		a.m[2] * a.m[4] * a.m[11] * a.m[13] +
		// m00 * m12 * m23 * m31 +
		a.m[0] * a.m[6] * a.m[11] * a.m[13] +
		// m03 * m11 * m20 * m32 -
		a.m[3] * a.m[5] * a.m[8] * a.m[14] -
		// m01 * m13 * m20 * m32 -
		a.m[1] * a.m[7] * a.m[8] * a.m[14] -
		// m03 * m10 * m21 * m32 + 
		a.m[3] * a.m[4] * a.m[9] * a.m[14] +
		// m00 * m13 * m21 * m32 +
		a.m[0] * a.m[7] * a.m[9] * a.m[14] +
		// m01 * m10 * m23 * m32 - 
		a.m[1] * a.m[4] * a.m[11] * a.m[14] -
		// m00 * m11 * m23 * m32 - 
		a.m[0] * a.m[5] * a.m[11] * a.m[14] -
		// m02 * m11 * m20 * m33 + 
		a.m[2] * a.m[5] * a.m[8] * a.m[15] +
		// m01 * m12 * m20 * m33 +
		a.m[1] * a.m[6] * a.m[8] * a.m[15] +
		// m02 * m10 * m21 * m33 - 
		a.m[2] * a.m[4] * a.m[9] * a.m[15] -
		// m00 * m12 * m21 * m33 - 
		a.m[0] * a.m[6] * a.m[9] * a.m[15] -
		// m01 * m10 * m22 * m33 + 
		a.m[1] * a.m[4] * a.m[10] * a.m[15] +
		// m00 * m11 * m22 * m33;
		a.m[0] * a.m[5] * a.m[10] * a.m[15];
	

	
	// COLLUM MAJOR
	/*return
		a.m[12] * a.m[9] * a.m[6] * a.m[3] -
		a.m[8] * a.m[13] * a.m[6] * a.m[3] -
		a.m[12] * a.m[5] * a.m[10] * a.m[3] +
		a.m[4] * a.m[13] * a.m[10] * a.m[3] +
		a.m[8] * a.m[5] * a.m[14] * a.m[3] -
		a.m[4] * a.m[9] * a.m[14] * a.m[3] -
		a.m[12] * a.m[9] * a.m[2] * a.m[7] +
		a.m[8] * a.m[13] * a.m[2] * a.m[7] +
		a.m[12] * a.m[1] * a.m[10] * a.m[7] -
		a.m[0] * a.m[13] * a.m[10] * a.m[7] -
		a.m[8] * a.m[1] * a.m[14] * a.m[7] +
		a.m[0] * a.m[9] * a.m[14] * a.m[7] +
		a.m[12] * a.m[5] * a.m[2] * a.m[11] -
		a.m[4] * a.m[13] * a.m[2] * a.m[11] -
		a.m[12] * a.m[1] * a.m[6] * a.m[11] +
		a.m[0] * a.m[13] * a.m[6] * a.m[11] +
		a.m[4] * a.m[1] * a.m[14] * a.m[11] -
		a.m[0] * a.m[5] * a.m[14] * a.m[11] -
		a.m[8] * a.m[5] * a.m[2] * a.m[15] +
		a.m[4] * a.m[9] * a.m[2] * a.m[15] +
		a.m[8] * a.m[1] * a.m[6] * a.m[15] -
		a.m[0] * a.m[9] * a.m[6] * a.m[15] -
		a.m[4] * a.m[1] * a.m[10] * a.m[15] +
		a.m[0] * a.m[5] * a.m[10] * a.m[15];*/
}


/* returns a 16-element array that is the inverse of a 16-element array (4x4
matrix). see http://www.euclideanspace.com/maths/algebra/matrix/functions/inverse/fourD/index.htm */
mat4 mat4::inverse(const mat4 & a) {

	float det = mat4::determinant(a);
	if (det == 0) { // epsilon
		return a;
	}
	float inv_det = 1.0f / det;
	mat4 b;

	
	b.m[0] = inv_det * (a.m[6] * a.m[11] * a.m[13] - a.m[7] * a.m[10] * a.m[13] + a.m[7] * a.m[9] * a.m[14] -
		a.m[5] * a.m[11] * a.m[14] - a.m[6] * a.m[9] * a.m[15] + a.m[5] * a.m[10] * a.m[15]);

	b.m[1] = inv_det * (a.m[3] * a.m[10] * a.m[13] - a.m[2] * a.m[11] * a.m[13] - a.m[3] * a.m[9] * a.m[14] +
		a.m[1] * a.m[11] * a.m[14] + a.m[2] * a.m[9] * a.m[15] - a.m[1] * a.m[10] * a.m[15]);

	b.m[2] = inv_det * (a.m[2] * a.m[7] * a.m[13] - a.m[3] * a.m[6] * a.m[13] + a.m[3] * a.m[5] * a.m[14] -
		a.m[1] * a.m[7] * a.m[14] - a.m[2] * a.m[5] * a.m[15] + a.m[1] * a.m[6] * a.m[15]);

	b.m[3] = inv_det * (a.m[3] * a.m[6] * a.m[9] - a.m[2] * a.m[7] * a.m[9] - a.m[3] * a.m[5] * a.m[10] +
		a.m[1] * a.m[7] * a.m[10] + a.m[2] * a.m[5] * a.m[11] - a.m[1] * a.m[6] * a.m[11]);

	b.m[4] = inv_det * (a.m[7] * a.m[10] * a.m[12] - a.m[6] * a.m[11] * a.m[12] - a.m[7] * a.m[8] * a.m[14] +
		a.m[4] * a.m[11] * a.m[14] + a.m[6] * a.m[8] * a.m[15] - a.m[4] * a.m[10] * a.m[15]);

	b.m[5] = inv_det * (a.m[2] * a.m[11] * a.m[12] - a.m[3] * a.m[10] * a.m[12] + a.m[3] * a.m[8] * a.m[14] -
		a.m[0] * a.m[11] * a.m[14] - a.m[2] * a.m[8] * a.m[15] + a.m[0] * a.m[10] * a.m[15]);

	b.m[6] = inv_det * (a.m[3] * a.m[6] * a.m[12] - a.m[2] * a.m[7] * a.m[12] - a.m[3] * a.m[4] * a.m[14] +
		a.m[0] * a.m[7] * a.m[14] + a.m[2] * a.m[4] * a.m[15] - a.m[0] * a.m[6] * a.m[15]);

	b.m[7] = inv_det * (a.m[2] * a.m[7] * a.m[8] - a.m[3] * a.m[6] * a.m[8] + a.m[3] * a.m[4] * a.m[10] -
		a.m[0] * a.m[7] * a.m[10] - a.m[2] * a.m[4] * a.m[11] + a.m[0] * a.m[6] * a.m[11]);

	b.m[8] = inv_det * (a.m[5] * a.m[11] * a.m[12] - a.m[7] * a.m[9] * a.m[12] + a.m[7] * a.m[8] * a.m[13] -
		a.m[4] * a.m[11] * a.m[13] - a.m[5] * a.m[8] * a.m[15] + a.m[4] * a.m[9] * a.m[15]);

	b.m[9] = inv_det * (a.m[3] * a.m[9] * a.m[12] - a.m[1] * a.m[11] * a.m[12] - a.m[3] * a.m[8] * a.m[13] +
		a.m[0] * a.m[11] * a.m[13] + a.m[1] * a.m[8] * a.m[15] - a.m[0] * a.m[9] * a.m[15]);

	b.m[10] = inv_det * (a.m[1] * a.m[7] * a.m[12] - a.m[3] * a.m[5] * a.m[12] + a.m[3] * a.m[4] * a.m[13] -
		a.m[0] * a.m[7] * a.m[13] - a.m[1] * a.m[4] * a.m[15] + a.m[0] * a.m[5] * a.m[15]);

	b.m[11] = inv_det * (a.m[3] * a.m[5] * a.m[8] - a.m[1] * a.m[7] * a.m[8] - a.m[3] * a.m[4] * a.m[9] +
		a.m[0] * a.m[7] * a.m[9] + a.m[1] * a.m[4] * a.m[11] - a.m[0] * a.m[5] * a.m[11]);

	b.m[12] = inv_det * (a.m[6] * a.m[9] * a.m[12] - a.m[5] * a.m[10] * a.m[12] - a.m[6] * a.m[8] * a.m[13] +
		a.m[4] * a.m[10] * a.m[13] + a.m[5] * a.m[8] * a.m[14] - a.m[4] * a.m[9] * a.m[14]);

	b.m[13] = inv_det * (a.m[1] * a.m[10] * a.m[12] - a.m[2] * a.m[9] * a.m[12] + a.m[2] * a.m[8] * a.m[13] -
		a.m[0] * a.m[10] * a.m[13] - a.m[1] * a.m[8] * a.m[14] + a.m[0] * a.m[9] * a.m[14]);

	b.m[14] = inv_det * (a.m[2] * a.m[5] * a.m[12] - a.m[1] * a.m[6] * a.m[12] - a.m[2] * a.m[4] * a.m[13] +
		a.m[0] * a.m[6] * a.m[13] + a.m[1] * a.m[4] * a.m[14] - a.m[0] * a.m[5] * a.m[14]);

	b.m[15] = inv_det * (a.m[1] * a.m[6] * a.m[8] - a.m[2] * a.m[5] * a.m[8] + a.m[2] * a.m[4] * a.m[9] -
		a.m[0] * a.m[6] * a.m[9] - a.m[1] * a.m[4] * a.m[10] + a.m[0] * a.m[5] * a.m[10]);
	

	return b;
	
	// Gauss-Jordan elimination
	// https://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/matrix-inverse/matrix-inverse

	/*for(unsigned column = 0; column < 4; ++column) {
		// Swap row in case our pivot point is not working
		if (a(column,column) == 0) {

			unsigned big = column;

			for (unsigned row = 0; row < 4; ++row)
				if (fabs(a(row,column)) > fabs(a(big,column))) big = row;

			// Print this is a singular matrix, return identity ?
			if (big == column) 
				fprintf(stderr, "Singular matrix\n");
			// Swap rows                               
			else for (unsigned j = 0; j < 4; ++j) {
				std::swap(a(column,j), a(big,j));
				std::swap(a(column,j), a(big,j));
			}
		}
		// Set each row in the column to 0  
		for (unsigned row = 0; row < 4; ++row) {

			if (row != column) {

				float coeff = a(row,column) / a(column,column);
				if (coeff != 0) {

					for (unsigned j = 0; j < 4; ++j) {
						a(row,j) -= coeff * a(column,j);
						a(row,j) -= coeff * a(column,j);
					}
					// Set the element to 0 for safety
					a(row,column) = 0;
				}

			}

		}
	}
	// Set each element of the diagonal to 1
	for (unsigned row = 0; row < 4; ++row) {
		for (unsigned column = 0; column < 4; ++column) {
			a(row,column) /= a(row,row);
		}
	}
	
	return a;*/

	// COLLUM MAJOR ORDER - computing determinants method
	/*float det = mat4::determinant(a);
	// there is no inverse if determinant is zero (not likely unless scale is broken) 
	if (0.0f == det) {
		//fprintf(stderr, "WARNING. matrix has no determinant. can not invert\n");
		return a;
	}
	float inv_det = 1.0f / det;
	return mat4(
		inv_det * (
		a.m[9] * a.m[14] * a.m[7] - a.m[13] * a.m[10] * a.m[7] +
		a.m[13] * a.m[6] * a.m[11] - a.m[5] * a.m[14] * a.m[11] -
		a.m[9] * a.m[6] * a.m[15] + a.m[5] * a.m[10] * a.m[15]
		),
		inv_det * (
		a.m[13] * a.m[10] * a.m[3] - a.m[9] * a.m[14] * a.m[3] -
		a.m[13] * a.m[2] * a.m[11] + a.m[1] * a.m[14] * a.m[11] +
		a.m[9] * a.m[2] * a.m[15] - a.m[1] * a.m[10] * a.m[15]
		),
		inv_det * (
		a.m[5] * a.m[14] * a.m[3] - a.m[13] * a.m[6] * a.m[3] +
		a.m[13] * a.m[2] * a.m[7] - a.m[1] * a.m[14] * a.m[7] -
		a.m[5] * a.m[2] * a.m[15] + a.m[1] * a.m[6] * a.m[15]
		),
		inv_det * (
		a.m[9] * a.m[6] * a.m[3] - a.m[5] * a.m[10] * a.m[3] -
		a.m[9] * a.m[2] * a.m[7] + a.m[1] * a.m[10] * a.m[7] +
		a.m[5] * a.m[2] * a.m[11] - a.m[1] * a.m[6] * a.m[11]
		),
		inv_det * (
		a.m[12] * a.m[10] * a.m[7] - a.m[8] * a.m[14] * a.m[7] -
		a.m[12] * a.m[6] * a.m[11] + a.m[4] * a.m[14] * a.m[11] +
		a.m[8] * a.m[6] * a.m[15] - a.m[4] * a.m[10] * a.m[15]
		),
		inv_det * (
		a.m[8] * a.m[14] * a.m[3] - a.m[12] * a.m[10] * a.m[3] +
		a.m[12] * a.m[2] * a.m[11] - a.m[0] * a.m[14] * a.m[11] -
		a.m[8] * a.m[2] * a.m[15] + a.m[0] * a.m[10] * a.m[15]
		),
		inv_det * (
		a.m[12] * a.m[6] * a.m[3] - a.m[4] * a.m[14] * a.m[3] -
		a.m[12] * a.m[2] * a.m[7] + a.m[0] * a.m[14] * a.m[7] +
		a.m[4] * a.m[2] * a.m[15] - a.m[0] * a.m[6] * a.m[15]
		),
		inv_det * (
		a.m[4] * a.m[10] * a.m[3] - a.m[8] * a.m[6] * a.m[3] +
		a.m[8] * a.m[2] * a.m[7] - a.m[0] * a.m[10] * a.m[7] -
		a.m[4] * a.m[2] * a.m[11] + a.m[0] * a.m[6] * a.m[11]
		),
		inv_det * (
		a.m[8] * a.m[13] * a.m[7] - a.m[12] * a.m[9] * a.m[7] +
		a.m[12] * a.m[5] * a.m[11] - a.m[4] * a.m[13] * a.m[11] -
		a.m[8] * a.m[5] * a.m[15] + a.m[4] * a.m[9] * a.m[15]
		),
		inv_det * (
		a.m[12] * a.m[9] * a.m[3] - a.m[8] * a.m[13] * a.m[3] -
		a.m[12] * a.m[1] * a.m[11] + a.m[0] * a.m[13] * a.m[11] +
		a.m[8] * a.m[1] * a.m[15] - a.m[0] * a.m[9] * a.m[15]
		),
		inv_det * (
		a.m[4] * a.m[13] * a.m[3] - a.m[12] * a.m[5] * a.m[3] +
		a.m[12] * a.m[1] * a.m[7] - a.m[0] * a.m[13] * a.m[7] -
		a.m[4] * a.m[1] * a.m[15] + a.m[0] * a.m[5] * a.m[15]
		),
		inv_det * (
		a.m[8] * a.m[5] * a.m[3] - a.m[4] * a.m[9] * a.m[3] -
		a.m[8] * a.m[1] * a.m[7] + a.m[0] * a.m[9] * a.m[7] +
		a.m[4] * a.m[1] * a.m[11] - a.m[0] * a.m[5] * a.m[11]
		),
		inv_det * (
		a.m[12] * a.m[9] * a.m[6] - a.m[8] * a.m[13] * a.m[6] -
		a.m[12] * a.m[5] * a.m[10] + a.m[4] * a.m[13] * a.m[10] +
		a.m[8] * a.m[5] * a.m[14] - a.m[4] * a.m[9] * a.m[14]
		),
		inv_det * (
		a.m[8] * a.m[13] * a.m[2] - a.m[12] * a.m[9] * a.m[2] +
		a.m[12] * a.m[1] * a.m[10] - a.m[0] * a.m[13] * a.m[10] -
		a.m[8] * a.m[1] * a.m[14] + a.m[0] * a.m[9] * a.m[14]
		),
		inv_det * (
		a.m[12] * a.m[5] * a.m[2] - a.m[4] * a.m[13] * a.m[2] -
		a.m[12] * a.m[1] * a.m[6] + a.m[0] * a.m[13] * a.m[6] +
		a.m[4] * a.m[1] * a.m[14] - a.m[0] * a.m[5] * a.m[14]
		),
		inv_det * (
		a.m[4] * a.m[9] * a.m[2] - a.m[8] * a.m[5] * a.m[2] +
		a.m[8] * a.m[1] * a.m[6] - a.m[0] * a.m[9] * a.m[6] -
		a.m[4] * a.m[1] * a.m[10] + a.m[0] * a.m[5] * a.m[10]
		)
		);*/
}



// returns a 16-element array flipped on the main diagonal
mat4 mat4::transpose(const mat4 & a) {


	/*
	x[0][0],x[1][0],x[2][0],x[3][0],
	x[0][1],x[1][1],x[2][1],x[3][1],
	x[0][2],x[1][2],x[2][2],x[3][2],
	x[0][3],x[1][3],x[2][3],x[3][3]);
	*/


	
return mat4(
		a.m[0], a.m[4], a.m[8], a.m[12],
		a.m[1], a.m[5], a.m[9], a.m[13],
		a.m[2], a.m[6], a.m[10], a.m[14],
		a.m[3], a.m[7], a.m[11], a.m[15]
		);
/*
 return mat4(
a.m[0], a.m[1], a.m[2], a.m[3],
a.m[4], a.m[5], a.m[6], a.m[7],
a.m[8], a.m[9], a.m[10], a.m[11],
a.m[12], a.m[13], a.m[14], a.m[15]
);
*/

}

/*mat4 mat4::operator* (const mat4& b) {


	return multiply(*this, b);


	/*for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {

			int index = i + j * 4;

			

			c[i][j] = a[i][0] * b[0][j] + a[i][1] * b[1][j] +
					  a[i][2] * b[2][j] + a[i][3] * b[3][j];
		}
	//}

	/*mat4 r = mat4::zero;
	int r_index = 0;
	for (int col = 0; col < 4; col++) {
		for (int row = 0; row < 4; row++) {
			float sum = 0.0f;
			for (int i = 0; i < 4; i++) {
				sum += b[i + col * 4] * m[row + i * 4];
			}
			r.m[r_index] = sum;
			r_index++;
		}
	}
	//return r;
}*/

const float& mat4::operator() (int i, int j) const 
{
	// i + j * 4

	return m[j + i * 4];
}

float& mat4::operator() (int i, int j) 
{
	return m[j + i * 4];
}

mat4& mat4::operator= (const mat4& b) {
	for (int i = 0; i < 16; i++) {
		m[i] = b[i];
	}
	return *this;
}

mat4 operator* (mat4 const & a, mat4 const & b) 
{
	return mat4::multiply(a, b);
}

vec4 operator* (mat4 const & a, vec4 const & v) 
{
	vec4 out = vec3::zero;

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {

			//int index = i + j * 4;

			out[i] += a(i, j) *  v[j];

		}
	}

	return out;

}

/*-----------------------VIRTUAL CAMERA MATRIX FUNCTIONS----------------------*/
// returns a view matrix using the opengl lookAt style. COLUMN ORDER.
/*mat4 look_at(const vec3& cam_pos, vec3 targ_pos, const vec3& up) {
	// inverse translation
	mat4 p = identity_mat4();
	p = translate(p, vec3(-cam_pos[0], -cam_pos[1], -cam_pos[2]));
	// distance vector
	vec3 d = targ_pos - cam_pos;
	// forward vector
	vec3 f = normalise(d);
	// right vector
	vec3 r = normalise(cross(f, up));
	// real up vector
	vec3 u = normalise(cross(r, f));
	mat4 ori = identity_mat4();
	ori.m[0] = r[0];
	ori.m[4] = r[1];
	ori.m[8] = r[2];
	ori.m[1] = u[0];
	ori.m[5] = u[1];
	ori.m[9] = u[2];
	ori.m[2] = -f[0];
	ori.m[6] = -f[1];
	ori.m[10] = -f[2];

	return ori * p;//p * ori;
}

// returns a perspective function mimicking the opengl projection style.
mat4 perspective(float fovy, float aspect, float near, float far) {
	float fov_rad = fovy * ONE_DEG_IN_RAD;
	float range = tan(fov_rad / 2.0f) * near;
	float sx = (2.0f * near) / (range * aspect + range * aspect);
	float sy = near / range;
	float sz = -(far + near) / (far - near);
	float pz = -(2.0f * far * near) / (far - near);
	mat4 m = zero_mat4(); // make sure bottom-right corner is zero
	m.m[0] = sx;
	m.m[5] = sy;
	m.m[10] = sz;
	m.m[14] = pz;
	m.m[11] = -1.0f;
	return m;
}*/

/*vec4 mat4::operator* (const vec4& rhs) {
	// 0x + 4y + 8z + 12w
	float x =
		m[0] * rhs[0] +
		m[4] * rhs[1] +
		m[8] * rhs[2] +
		m[12] * rhs[3];
	// 1x + 5y + 9z + 13w
	float y = m[1] * rhs[0] +
		m[5] * rhs[1] +
		m[9] * rhs[2] +
		m[13] * rhs[3];
	// 2x + 6y + 10z + 14w
	float z = m[2] * rhs[0] +
		m[6] * rhs[1] +
		m[10] * rhs[2] +
		m[14] * rhs[3];
	// 3x + 7y + 11z + 15w
	float w = m[3] * rhs[0] +
		m[7] * rhs[1] +
		m[11] * rhs[2] +
		m[15] * rhs[3];
	return vec4(x, y, z, w);
}*/