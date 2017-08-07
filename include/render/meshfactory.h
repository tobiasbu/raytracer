
#include "mesh.h"

#ifndef _MESHFACTORY_H_
#define _MESHFACTORY_H_

static class MeshFactory {

public:
	static Mesh * quad(float width = 5, float height = 5) {

		float wh = width / 2;
		float hh = height / 2;

		

		//vec3 verts[4] = { { -5, -1, -6 }, { 5, -1, -6 }, { 5, -1, -16 }, { -5, -1, -16 } };

		//std::vector<vec3> vert(verts, verts + sizeof(verts) / sizeof(verts[0]));
		std::vector<vec3> vert;

		vert.push_back(vec3(-wh, 0, hh));
		vert.push_back(vec3(wh, 0, hh));
		vert.push_back(vec3(wh, 0, -hh));
		vert.push_back(vec3(-wh, 0, -hh));

		uint32_t tmp[6] = { 0, 1, 3, 1, 2, 3 };
		std::vector<uint32_t> triangles(tmp, tmp + sizeof(tmp) / sizeof(tmp[0]));
		
		vec2 tmpSt[4] = { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 0, 1 } };

		/*
			0---1
			|   |
			3---2
		*/
		std::vector<vec2> st(tmpSt, tmpSt + sizeof(tmpSt) / sizeof(tmpSt[0]));
		std::vector<vec3> n;

		/*n.push_back(vec3(0, -1, 0));
		n.push_back(vec3(0, -1, 0));
		n.push_back(vec3(0, -1, 0));
		n.push_back(vec3(0, -1, 0));*/

		Mesh * mesh = new Mesh(triangles, vert,n, st);

	

		return mesh;
	}

};

#endif