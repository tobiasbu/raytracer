
#include "render\mesh.h"
#include "math/maths.h"

Mesh::Mesh(const std::vector<uint32_t> & triangleIndex,
	/*std::vector<uint32_t> & vertsIndex,*/
	const std::vector<vec3> & vertices,
	const std::vector<vec3> & normals,
	const std::vector<vec2> & texCoords,
	const int & useBVH
	) 
{
	_type = PrimitiveType::Mesh;
	_triangleIndex = triangleIndex;
	_numTriangles = triangleIndex.size() / 3;
	_useBVH = useBVH;

	// slow way
	if (useBVH < 2)
	{

		_vertices = vertices;
		_normals = normals;
		_texCoords = texCoords;

		_bounds.set(0, 0);

		// resize bounding box
		for (size_t i = 0; i < vertices.size(); i++)
		{
			_bounds = _bounds.extend(vertices[i]);
		}
		

		computeTangents();
	}
	else // use bvh for mesh
	{
		

		bool hasTexCoord = true;
		bool hasNormals = true;

		if (texCoords.empty())
			hasTexCoord = false;

		if (normals.empty())
			hasNormals = false;

		int32_t triIndex = 0;

		for (uint32_t i = 0; i < _numTriangles; ++i) {

			triIndex = i * 3;

			/*const vec3 v[] = {
				vertices[_triangleIndex[triIndex]],
				vertices[_triangleIndex[triIndex + 1]],
				vertices[_triangleIndex[triIndex + 2]]
			};*/

			Triangle * tri = new Triangle;
			tri->_index = i;

			tri->_vertices[0] = vertices[triangleIndex[triIndex]];
			tri->_vertices[1] = vertices[triangleIndex[triIndex + 1]];
			tri->_vertices[2] = vertices[triangleIndex[triIndex + 2]];

			if (hasTexCoord) 
			{
				tri->_texcoords[0] = texCoords[triangleIndex[triIndex]];
				tri->_texcoords[1] = texCoords[triangleIndex[triIndex + 1]];
				tri->_texcoords[2] = texCoords[triangleIndex[triIndex + 2]];
			}

			tri->hasNormals = hasNormals;

			if (hasNormals)
			{
				
				tri->_normals[0] = normals[triangleIndex[triIndex]];
				tri->_normals[1] = normals[triangleIndex[triIndex + 1]];
				tri->_normals[2] = normals[triangleIndex[triIndex + 2]];
			}

			tri->initialize();

			_bounds = _bounds.extend(tri->getBounds());

			_triangles.push_back(tri);

			
		}

		

	}
	
}



const std::vector<vec3> & Mesh::getVertices() const
{
	return _vertices;
}

bool Mesh::intersects(const Ray & ray, RaycastHit & hit, float & tNear) {

	//Ray ray = rayt;
	// transform ray
	//ray.transform(getInversedMatrix());

	bool intersect = false;

	if (_useBVH < 2) {

		uint32_t triIndex = 0;
		
		float t, u, v;

		for (uint32_t i = 0; i < _numTriangles; ++i) {

			triIndex = i * 3;

			const vec3 &v0 = _vertices[_triangleIndex[triIndex]];
			const vec3 &v1 = _vertices[_triangleIndex[triIndex + 1]];
			const vec3 &v2 = _vertices[_triangleIndex[triIndex + 2]];


			//std::cout << _triangleIndex[j] << std::endl;
			//std::cout << v0 << v1 << v2 << std::endl;
			hit.tests++;
			if (rayTriangleIntersects(ray, v0, v1, v2, t, u, v) && t < tNear) {
				tNear = t;
				//hit.distance = tNear;
				hit.barycentric.x = u;
				hit.barycentric.y = v;
				hit.triangle = triIndex;
				intersect = true;

			}
			//j += 3;
		}
	}
	else 
	{
		if (_bvh->intersects(ray, hit) )
		{
			intersect = true;
			//hit.distance
			hit.object = this;
		}
	}

	return intersect;

}

bool Mesh::rayTriangleIntersects(const Ray & ray, const vec3 & v0, const vec3 & v1, const vec3 & v2, float & t, float & u, float & v) const {


	vec3 edge1 = v1 - v0;
	vec3 edge2 = v2 - v0;
	vec3 pvec = vec3::cross(ray.direction, edge2); // dir.crossProduct(v0v2);
	float det = vec3::dot(edge1, pvec);

	// ray and triangle are parallel if det is close to 0
	if (fabs(det) < Epsilon) // fEpsilon
		return false;

	float invDet = 1 / det;

	vec3 tvec = ray.origin - v0;
	u = vec3::dot(tvec, pvec) * invDet;

	if (u < 0 || u > 1)
		return false;

	vec3 qvec = vec3::cross(tvec, edge1);
	v = vec3::dot(ray.direction, qvec) * invDet;

	if (v < 0 || u + v > 1)
		return false;

	

	t = vec3::dot(edge2, qvec) * invDet;



	return true;
}

void Mesh::computeSurfaceData(RaycastHit & hit) {

	if (_useBVH < 2) {
		//if (!_texCoords.empty()) {
		const vec3 &v0 = _vertices[_triangleIndex[hit.triangle]];
		const vec3 &v1 = _vertices[_triangleIndex[hit.triangle + 1]];
		const vec3 &v2 = _vertices[_triangleIndex[hit.triangle + 2]];



		if (_normals.empty()) {
			// flat shading
			hit.normal = vec3::cross((v1 - v0).normalized(), (v2 - v1).normalized());
			//vec3::cross(v1 - v0, v2 - v1);
			// + hit.normal;
		}
		else
		{
			// smooth shading
			const vec3 &n0 = _normals[_triangleIndex[hit.triangle]];
			const vec3 &n1 = _normals[_triangleIndex[hit.triangle + 1]];
			const vec3 &n2 = _normals[_triangleIndex[hit.triangle + 2]];
			hit.normal = (1 - hit.barycentric.x - hit.barycentric.y) * n0 + hit.barycentric.x * n1 + hit.barycentric.y * n2;
		}


		hit.normal.normalize();

		hit.normal *= -1;

		if (_tangents.size() > 0)
			hit.tangent = _tangents[hit.triangle / 3];

		/*mat4 inv = getInversedMatrix();
		inv = mat4::transpose(inv);

		n = inv * vec4(n, 0);

		hit.normal = n.normalized();*/
		//hit.normal.normalize();

		// smooth shading
		/*const vec3 &n0 = _normals[hit.triangle];
		const vec3 &n1 = _normals[hit.triangle + 1];
		const vec3 &n2 = _normals[hit.triangle + 2];
		hit.normal = (1 - hit.texCoord.x - hit.texCoord.y) * n0 + hit.texCoord.x * n1 + hit.texCoord.y * n2;*/

		if (!_texCoords.empty()) {
			const vec2 &st0 = _texCoords[_triangleIndex[hit.triangle]];
			const vec2 &st1 = _texCoords[_triangleIndex[hit.triangle + 1]];
			const vec2 &st2 = _texCoords[_triangleIndex[hit.triangle + 2]];
			/*const vec2 &st0 = _texCoords[hit.triangle];
			const vec2 &st1 = _texCoords[hit.triangle + 1];
			const vec2 &st2 = _texCoords[hit.triangle + 2];*/
			hit.texCoord = (1 - hit.barycentric.x - hit.barycentric.y) * st0 + hit.barycentric.x * st1 + hit.barycentric.y * st2;


			if (hit.texCoord.x < 0)
				hit.texCoord.x *= -1;
			if (hit.texCoord.y < 0)
				hit.texCoord.y *= -1;
		}

	}
	else 
	{
		_triangles[hit.triangle]->computeSurfaceData(hit);
	}
}

void Mesh::updateTransform() 
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

	Bounds aabb;


	mat4 transformNormals = inversedMatrix.transposed();

	if (_useBVH < 2) {

		for (size_t i = 0; i < _vertices.size(); i++) {

			_vertices[i] = MatrixTransform::multiplyVec(matrix, _vertices[i]);

			if (_normals.size() > 0 && i < _normals.size())
				_normals[i] = MatrixTransform::multiplyPoint(transformNormals, _normals[i]);
			aabb = aabb.extend(_vertices[i]);


		}

	}
	else 
	{
		for (size_t i = 0; i < _triangles.size(); i++) 
		{
			static_cast<Triangle*>(_triangles[i])->updateTransform(matrix, transformNormals);
			aabb = Bounds::merge(aabb,_triangles[i]->getBounds());
		}
	}

	_bounds = aabb;


}

bool Mesh::computeTangents()
{
	if (_texCoords.size() == 0)
		return false;

	size_t triIndex;
	int lastTri;

		for (uint32_t i = 0; i < _numTriangles; ++i) {

			triIndex = i * 3;


			/*if (i == _numTriangles - 1)
				lastTri = 0;
			else
				lastTri = triIndex + 2;*/
				


			const vec3 &v0 = _vertices[_triangleIndex[triIndex]];
			const vec3 &v1 = _vertices[_triangleIndex[triIndex + 1]];
			const vec3 &v2 = _vertices[_triangleIndex[triIndex + 2]];

			const vec3 &uv0 = _texCoords[_triangleIndex[triIndex]];
			const vec3 &uv1 = _texCoords[_triangleIndex[triIndex + 1]];
			const vec3 &uv2 = _texCoords[_triangleIndex[triIndex + 2]];
		

			

			// triangle's edges and delta UV coordinates:
			vec3 edge1 = v1 - v0;
			vec3 edge2 = v2 - v0;
			vec2 deltaUV1 = uv1 - uv0;
			vec2 deltaUV2 = uv2 - uv0;

			float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

			vec3 tangent;

			tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
			tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
			tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
			tangent.normalize();

			// every triagle has one tangent
			_tangents.push_back(tangent);
		}

		return true;

}

void Mesh::buildBVH()
{
	if (_useBVH >= 2) 
	{

		BVH::SplitMethod split = BVH::SplitMethod::Middle;

		if (_useBVH == 3)
			split = BVH::SplitMethod::SAH;

		_bvh = new BVH(this, split);
		_bvh->build();

	}
}