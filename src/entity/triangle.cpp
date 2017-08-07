
#include "entity/triangle.h"

Triangle::Triangle(const uint32_t & index, vec3 vertices[3], vec2 * texCoord, vec3 * normals, vec3 * tangent)
{
	_type = PrimitiveType::_Triangle;

	_index = index;

	_vertices[0] = vertices[0];
	_vertices[1] = vertices[1];
	_vertices[2] = vertices[2];

	_bounds.extend(_vertices[0]);
	_bounds.extend(_vertices[1]);
	_bounds.extend(_vertices[2]);

	_texcoords[0] = texCoord[0];
	_texcoords[1] = texCoord[1];
	_texcoords[2] = texCoord[2];

	vec3 v0tov1 = vertices[1] - vertices[0];
	vec3 v0tov2 = vertices[2] - vertices[0];

	if (normals == nullptr)
	{
		
		_normal = vec3::cross(v0tov1,v0tov2).normalized();
		hasNormals = false;
	}
	else 
	{
		_normal = (*normals);
		hasNormals = true;
	}

	if (tangent == nullptr)
	{
		vec2 t0tot1 = _texcoords[1] - _texcoords[0];
		vec2 t0tot2 = _texcoords[2] - _texcoords[0];
		double denominator = t0tot1.x * t0tot2.y - t0tot1.y * t0tot2.x;

		if (std::abs(denominator) > std::numeric_limits<double>::epsilon())
		{
			double r = 1.0 / denominator;
			_tangent = (v0tov1 * t0tot2.y - v0tov2 * t0tot1.y) * r;
			//bitangent = (v0tov2 * t0tot1.x - v0tov1 * t0tot2.x) * r;
			_tangent.normalize();
			//bitangent.normalize();
		}
		else
		{
			// almost up
			_tangent = vec3::cross(_normal, vec3(0.0001f, 1.0f, 0.0001f)).normalized();
		}
	}
	else
	{
		_tangent = (*tangent);
	}
}

void Triangle::initialize()
{
	_bounds = Bounds();
	_bounds = Bounds::merge(_bounds, _vertices[0]);
	_bounds = Bounds::merge(_bounds, _vertices[1]);
	_bounds = Bounds::merge(_bounds, _vertices[2]);

	vec3 v0tov1 = _vertices[1] - _vertices[0];
	vec3 v0tov2 = _vertices[2] - _vertices[0];

	if (!hasNormals) 
	{
		_normal = vec3::cross(v0tov1, v0tov2).normalized();
	}

	vec2 t0tot1 = _texcoords[1] - _texcoords[0];
	vec2 t0tot2 = _texcoords[2] - _texcoords[0];
	double denominator = t0tot1.x * t0tot2.y - t0tot1.y * t0tot2.x;

	if (std::abs(denominator) > std::numeric_limits<double>::epsilon())
	{
		double r = 1.0 / denominator;
		_tangent = (v0tov1 * t0tot2.y - v0tov2 * t0tot1.y) * r;
		//bitangent = (v0tov2 * t0tot1.x - v0tov1 * t0tot2.x) * r;
		_tangent.normalize();
		//bitangent.normalize();
	}
	else
	{
		// almost up
		_tangent = vec3::cross(_normal, vec3(0.0001f, 1.0f, 0.0001f)).normalized();
	}

}

void Triangle::updateTransform() 
{
	mat4 s = MatrixTransform::scale(mat4::identity, scale);
	mat4 t = MatrixTransform::translate(mat4::identity, position);
	mat4 r = MatrixTransform::rotate_x_deg(mat4::identity, angle.x);

	r = MatrixTransform::rotate_y_deg(r, angle.y);
	r = MatrixTransform::rotate_z_deg(r, angle.z);

	// transform o row-major is T * R * S
	//  collum:
	//matrix = t;
	//matrix = s * r * t;
	matrix = t *r * s;
	inversedMatrix = mat4::inverse(matrix);
	calculateVectors();

	_bounds.set(0, 0);

	mat4 transformNormals = inversedMatrix.transposed();

	for (size_t i = 0; i < 3; i++) {

		_vertices[i] = MatrixTransform::multiplyVec(matrix, _vertices[i]);
		_bounds = _bounds.extend(_vertices[i]);


	}

	_normal = MatrixTransform::multiplyPoint(transformNormals, _normal);
}

void Triangle::updateTransform(const mat4 & matrix, const mat4 & matrixNormal)
{
	//Bounds aabb;
	
	for (size_t i = 0; i < 3; i++) {

		_vertices[i] = MatrixTransform::multiplyVec(matrix, _vertices[i]);
		//aabb = Bounds::merge(aabb,_vertices[i]);

	}

	_bounds = Bounds(_vertices[0], _vertices[1], _vertices[2]);

	if (hasNormals) 
	{
		_normals[0] = MatrixTransform::multiplyPoint(matrixNormal, _normals[0]);
		_normals[1] = MatrixTransform::multiplyPoint(matrixNormal, _normals[1]);
		_normals[2] = MatrixTransform::multiplyPoint(matrixNormal, _normals[2]);
	}
	else 
	{
		_normal = MatrixTransform::multiplyPoint(matrixNormal, _normal);
	}

}

bool Triangle::intersects(const Ray & ray, RaycastHit & hit, float & t)
{
	hit.tests++;
	vec3 edge1 = _vertices[1] - _vertices[0];
	vec3 edge2 = _vertices[2] - _vertices[0];
	vec3 pvec = vec3::cross(ray.direction, edge2); // dir.crossProduct(v0v2);
	float det = vec3::dot(edge1, pvec);

	// ray and triangle are parallel if det is close to 0
	if (fabs(det) < Epsilon) // fEpsilon
		return false;

	float invDet = 1 / det;

	vec3 tvec = ray.origin - _vertices[0];
	float u = vec3::dot(tvec, pvec) * invDet;

	if (u < 0.0f || u > 1.0f)
		return false;

	vec3 qvec = vec3::cross(tvec, edge1);
	float v = vec3::dot(ray.direction, qvec) * invDet;

	if (v < 0.0f || (u + v )> 1.0f)
		return false;


	float tt = vec3::dot(edge2, qvec) * invDet;

	if (tt < 0.0)
		return false;

	if (tt > hit.distance)
		return false;

	t = tt;
	//hit.distance = t;
	hit.barycentric.x = u;
	hit.barycentric.y = v;
	hit.triangle = _index;

	return true;
}

void Triangle::computeSurfaceData(RaycastHit & hit)
{
	/*const vec3 &v0 = _vertices[_triangleIndex[hit.triangle]];
	const vec3 &v1 = _vertices[_triangleIndex[hit.triangle + 1]];
	const vec3 &v2 = _vertices[_triangleIndex[hit.triangle + 2]];*/

	float w = (1.0 - hit.barycentric.x - hit.barycentric.y);

	if (hasNormals)
		hit.normal = w * _normals[0] + hit.barycentric.x * _normals[1] + hit.barycentric.y * _normals[2];
	else 
	{
		hit.normal = _normal;
		//vec3::cross((v1 - v0).normalized(), (v2 - v1).normalized());
	}
	hit.normal.normalize();
	hit.normal = -hit.normal;
	hit.texCoord = w * _texcoords[0] + hit.barycentric.x * _texcoords[1] + hit.barycentric.y * _texcoords[2];

	if (hit.texCoord.x < 0)
		hit.texCoord.x *= -1;
	if (hit.texCoord.y < 0)
		hit.texCoord.y *= -1;

	hit.tangent = _tangent;

		
}