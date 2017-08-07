
#include "entity/plane.h"

Plane::Plane() {

	_normal = vec3(0, 1, 0);
	_offset = 1;
	_type = PrimitiveType::Plane;
	//_bounds.set()
}

Plane::Plane(vec3 normal, float offset) {

	_normal = normal;
	_offset = offset;
	_type = PrimitiveType::Plane;
}

void Plane::setNormal(const vec3 & n)
{
	_normal = n;
}

void Plane::setOffset(const float & o)
{
	_offset = o;
}

vec3 Plane::getNormal() {

	return _normal;

}

bool Plane::intersects(const Ray & rray, RaycastHit & hit, float & t) {

	Ray ray = rray;

	// transform ray
	ray.transform(getInversedMatrix());

	// intersection with point-normal of the plane
	//  plane formula A x * B y * C z + D = 0;
	//  ray formula   x = a * t + a0, y = b * t + b0, z = c * t + c0;

	// d is offset
	// t = -(origin . normal + d) / (direction . normal)

	// direction dot normal
	float a = vec3::dot(_normal, ray.direction);

	// check if is negative
	if (a <= 0)
		return false;
	else {

		
		/*vec3 n = vec3(_normal * _offset).negative();
		n.add(ray.origin);
		float b = vec3::dot(_normal, n);
		t = -1.f * b / a;*/
		t = (_offset - vec3::dot(_normal, ray.origin)) / a;
		return true;
	}

	



}

void Plane::computeSurfaceData(RaycastHit & hit)
{
	hit.normal = hit.point - (_offset * _normal);
	hit.normal.normalize();

	//hit.normal *= -1;

	vec3 n = -_normal;


	vec3 uAxis = n;//vec3(n.y, n.z, n.x) ;
	vec3 vAxis = vec3::cross(uAxis, n);

	hit.texCoord = vec2(vec3::dot(hit.point, uAxis), vec3::dot(hit.point, vAxis) );

}

void Plane::updateTransform() {

	matrix = mat4::identity;
	mat4 s = MatrixTransform::scale(mat4::identity, scale);
	mat4 t = MatrixTransform::translate(mat4::identity, position);
	mat4 r = MatrixTransform::rotate_x_deg(mat4::identity, angle.x);
	r = MatrixTransform::rotate_y_deg(r, angle.y);
	r = MatrixTransform::rotate_z_deg(r, angle.z);


	matrix = t * s * r;
	inversedMatrix = mat4::inverse(matrix);
	calculateVectors();

	//mat4 trans = inversedMatrix.transposed();

	//_normal = MatrixTransform::multiplyPoint(trans, _normal);

	//_bounds.set(_normal - _offset, _normal + _offset);

	// compute inversed matrix
	

}

