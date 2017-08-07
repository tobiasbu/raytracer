
#include "entity/sphere.h"
#include "math/maths.h"

#include <algorithm>

Sphere::Sphere()
{

	_radius = 1.0;
	_radius2 = _radius * _radius;
	_type = PrimitiveType::Sphere;
	
	_bounds.set(position + -_radius, position + _radius);
	
}

Sphere::Sphere(float radius, vec3 position)
{

	_radius = radius;
	_radius2 = radius *radius;
	_type = PrimitiveType::Sphere;
	this->position = position;
	_bounds.set(position -_radius, position + _radius);
	
}

void Sphere::setRadius(float r)
{
	_radius = r;
	_radius2 = _radius * _radius;
	_bounds.set(position + -_radius, position + _radius);
}

float Sphere::getRadius()
{

	return _radius;

}

bool Sphere::intersects(const Ray & ray, RaycastHit & hit, float & tNear)
{ 
	// transform ray
	
	//mat4 t = mat4(1.0);
	
	

	float t0, t1; // solutions for t if the ray intersects 

//#define A 
#if 0
	// geometric solution
	vec3 L = position - ray.origin;
	float tca = vec3::dot(L,ray.direction);
	//if (tca < 0) 
	//	return false;
	float d2 = vec3::dot(L,L) - tca * tca;
	if (d2 > _radius2) 
		return false;
	float thc = sqrt(_radius2 - d2);
	t0 = tca - thc;
	t1 = tca + thc;
#elsif 0
	// TRANSFORM SOLUTION 1 - COMPUTING CAMERA TRANSFORM * OBJECT TRANSFORM
	// model * viewMatrix
	// analytic solution
	vec3 L = ray.origin - (position); //- (_radius * 0.5))/*center*/;
	float a = vec3::dot(ray.direction, ray.direction); //dir.dotProduct(dir);
	float b = 2 * vec3::dot(ray.direction, L);
	float c = vec3::dot(L, L) - (_radius2);
	if (!Math::solveQuadratic(a, b, c, t0, t1))
		return false;
//#endif
	#else

	// TRANSFORM SOLUTION 2
	// without computing model matrix * view matrix.
	// it's need only transfrom ray by model matrix

	
	Ray transformedRay = ray;

	// transform ray
	transformedRay.transform(getInversedMatrix());

	
	//vec3 L = transformedRay.origin - (position); // (dont need this line)
	float a = vec3::dot(transformedRay.direction, transformedRay.direction); //dir.dotProduct(dir);
	float b = 2 * vec3::dot(transformedRay.direction, transformedRay.origin);
	float c = vec3::dot(transformedRay.origin, transformedRay.origin) - _radius2; // - _radius2; // -1
	if (!Math::solveQuadratic(a, b, c, t0, t1))
		return false;

#endif 

	if (t0 > t1) 
		std::swap(t0, t1);

	if (t0 < 0) {
		t0 = t1; // if t0 is negative, let's use t1 instead 
		if (t0 < 0) 
			return false; // both t0 and t1 are negative 
	}

	
	tNear = t0;


	return true; 
}

void Sphere::computeSurfaceData(RaycastHit & hit)
{
	// calculate normal and texture coordinates

	//////////////////////// 
	// SOLUTION 1 
	
	// using transform that has previous operation: model matrix * view matrix
	// check 'transform.h' and 'scene.h' -> updateTransform functions

	/*vec3 n = hit.point - position;
	n.normalize();

	hit.normal = n;
	hit.texCoord.x = (1 + atan2(n.z, n.x) / PI) * 0.5;
	hit.texCoord.y = acosf(n.y) / PI;*/

	////////////////////////// 
	// SOLUTION 2 

	// using only transform model matrix

	// compute normal (local normal)
	vec3 n = hit.point - position;
	n.normalize();

	hit.texCoord = computeTexCoord(n);
	// compute texcoord by up, right forward vectors
	

	// must get inversed matrix4 
	// recalculate normal - to world normal
	mat4 inv = getInversedMatrix();
	inv = mat4::transpose(inv);
	
	n = inv * vec4(n,0);
	
	

	hit.normal = -n;

	
	

	

	hit.tangent = vec3::cross(n, hit.ray->direction).normalized();

	//hit.tangent = vec3::cross(n, MatrixTransform::multiplyPoint(inv,hit.ray->direction)).normalized();
	

	// OLD
	//hit.normal = hit.point - position;// - (_radius * 0.5));
	
	//vec3 n = (2 * (hit.point - position)).normalized();
	//if (_material != nullptr && _material->getTexuteID() > -1)
	//hit.texCoord = computeTexCoord(hit.point);
	// calculate texcoord
	//hit.set(normal, L, t);
	//hit.object = this;*

	
	/*nhit = phit - transform.position;
	nhit.normalized();
	// In this particular case, the normal is simular to a point on a unit sphere
	// centred around the origin. We can thus use the normal coordinates to compute
	// the spherical coordinates of Phit.
	// atan2 returns a value in the range [-pi, pi] and we need to remap it to range [0, 1]
	// acosf returns a value in the range [0, pi] and we also need to remap it to the range [0, 1]
	vec2 tex;
	tex.x = (1 + atan2(nhit.z, nhit.x) / M_PI) * 0.5;
	tex.y = acosf(nhit.y) / M_PI;
	hit.texCoord.x = (1 + atan2(hit.normal.z, hit.normal.x) / PI) * 0.5;
	hit.texCoord.y = acosf(hit.normal.y) / PI;

	return tex;
	*/


}

vec2 Sphere::computeTexCoord(vec3 & point)
{
	// about __restrict:
	// http://cellperformance.beyond3d.com/articles/2006/05/demystifying-the-restrict-keyword.html

	
	//const vec3 & __restrict center = position;// - (_radius * 0.5);
	
	vec3 p = (point - position) / _radius; // (1.0f / _radius); // * (_radius * 2); //(1.0f / _radius);
	//p.normalize();

	float coeff = 1.f / PI;
	float phi = acosf(vec3::dot(up, p)); //(_radius /* (PI * 0.5f))*/);

	float theta = (acosf(-vec3::dot(right, p) / sinf(phi)))  * (coeff / 2);//(0.5f / PI); // / (2 * PI); // * (0.5f/PI); // / 2 * PI); // / 2*pi
	float u, v;
	v = phi * coeff;//(1.0f / PI); //  / pi


	if (vec3::dot(forward, p) >= 0)
		u = (1.0f - theta); //* m_Material->GetUScaleReci();
	else
		u = theta; // *m_Material->GetUScaleReci();*/


	/*float phi = acosf(-DOT(vp, m_Vn));
	float u, v = phi * m_Material->GetVScaleReci() * (1.0f / PI);
	float theta = (acosf(DOT(m_Ve, vp) / sinf(phi))) * (2.0f / PI);
	if (DOT(m_Vc, vp) >= 0) u = (1.0f - theta) * m_Material->GetUScaleReci();
	else u = theta * m_Material->GetUScaleReci();*/

	//center = (position - (_radius * 0.5));

	//float theta = acos((point.z - center.z) / _radius);
	//float phi = atan2(point.y - center.y, point.x - center.x);
	/*float theta = acos(p.z / _radius);
	float phi = atan2(p.y, p.x);

	if (phi < 0) 
		phi = phi + 2 * PI;
	

	float u = phi / (2 * PI);
	float v = (PI - theta) / PI;*/

	// tc.print();
	return vec2(u,v);

}

void Sphere::updateTransform() {

	matrix = mat4::identity;
	mat4 s = MatrixTransform::scale(mat4::identity, scale);
	mat4 t = MatrixTransform::translate(mat4::identity, position);
	mat4 r = MatrixTransform::rotate_x_deg(mat4::identity, angle.x);

	r = MatrixTransform::rotate_y_deg(r, angle.y);
	r = MatrixTransform::rotate_z_deg(r, angle.z);

	// transform o row-major is T * R * S
	//  collum:
	//matrix = s * r * t;

	matrix = s * r * t;//t * r * s;
	calculateVectors();
	//matrix = s * r * t;
	
	// another solution: precompute transformed object: only model matrix
	

	_bounds = Bounds::merge(Bounds(), position - (scale*_radius));
	_bounds = Bounds::merge(_bounds, position + (_radius*scale));
	//, position + (_radius*scale));

	// compute inversed matrix
	inversedMatrix = mat4::inverse(matrix);
	



}