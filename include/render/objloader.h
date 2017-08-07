
#ifndef _OBJLOADER_H_
#define _OBJLOADER_H_

#include <vector>
#include <string>
#include <map>
#include "math/vec2.h"
#include "math/vec3.h"

class ObjLoader {

public:

	enum TextureType 
	{
		TEXTURE_TYPE_NONE,  // default
		TEXTURE_TYPE_SPHERE,
		TEXTURE_TYPE_CUBE_TOP,
		TEXTURE_TYPE_CUBE_BOTTOM,
		TEXTURE_TYPE_CUBE_FRONT,
		TEXTURE_TYPE_CUBE_BACK,
		TEXTURE_TYPE_CUBE_LEFT,
		TEXTURE_TYPE_CUBE_RIGHT
	};

	struct VertexIndex
	{
		// -1 means no normals and no texcoord
		int vertex;
		int normal;
		int texcoord;

		VertexIndex()
			:
			vertex(-1),
			normal(-1),
			texcoord(-1)
		{

		}
		VertexIndex(int vertex, int normal, int texcoord)
		{
			this->vertex = vertex;
			this->normal = normal;
			this->texcoord = texcoord;
		}

		bool operator ==(const VertexIndex & other)
		{
			return (this->vertex == other.vertex);
		}

		bool operator !=(const VertexIndex & other)
		{
			return (this->vertex != other.vertex);
		}

		friend std::ostream& operator << (std::ostream &s, const VertexIndex &v)
		{
			return s << "vertex (" << v.vertex << "\t" << v.texcoord << "\t" << v.normal << ')';
		}
	};

	// TODO use only vertex index, instead face
	struct Face {
		std::vector<VertexIndex> vertexes;
		int material;

		Face()
			:
			material(-1)
		{

		}

	};

	struct Texture {
		std::string name;
		TextureType type;
		float sharpness;         // -boost (default 1.0?)
		float brightness;        // base_value in -mm option (default 0)
		float contrast;          // gain_value in -mm option (default 1)
		vec3 offset;  // -o u [v [w]] (default 0 0 0)
		vec3 scale;          // -s u [v [w]] (default 1 1 1)
		vec3 turbulence;     // -t u [v [w]] (default 0 0 0)
		// int   texture_resolution; // -texres resolution (default = ?) TODO
		bool clamp;    // -clamp (default false)
		char imfchan;  // -imfchan (the default for bump is 'l' and for decal is 'm')
		bool blendu;   // -blendu (default on)
		bool blendv;   // -blendv (default on)
		float bumpMultiplier;  // -bm (for bump maps only, default 1.0)

		Texture()
		{
			reset();
		}

		void reset() 
		{
			name = "";
			type = TEXTURE_TYPE_NONE;
			clamp = false;
			sharpness = 0;
			brightness = 1;
			scale = vec3(1, 1, 1);
			offset = vec3(0, 0, 0);
			bumpMultiplier = 1;
		}
	};

	struct Material 
	{
		std::string name;
		vec3 ka;
		vec3 kd;
		vec3 ks;
		vec3 kt;
		vec3 ke;
		float ns;
		float ni; // refraction
		int illum;
		float d;
		Texture map_ka;
		Texture map_kd;
		Texture map_ks;
		Texture map_ns;
		Texture map_bump;
		Texture map_d;
		// disp
	

		Material()
		{
			reset();
		}

		void reset()
		{
			name = "";

			ka = vec3::zero;
			kd = vec3::zero;
			ks = vec3::zero;
			kt = vec3::zero;
			ke = vec3::zero;
			ns = 1;
			ni = 0;
			illum = 0;
			d = 1;
			// reset textures
			map_ka.reset();
			map_kd.reset();
			map_ks.reset();
			map_ns.reset();
			map_bump.reset();
			map_d.reset();
		}
	};

	struct Mesh 
	{
		std::string name;
		std::vector<unsigned int> indices;
		std::vector<vec3> vertices;
		std::vector<vec2> texcoord;
		std::vector<vec3> normals;
		int materialID;

		Mesh()
			:
			materialID(-1)
		{}
	};

	struct Model 
	{
		std::vector<Material> materials;
		std::vector<Mesh *> meshes;
		std::string name;
		std::string path;
	};

	// Load Wavefront Obj file
	// /param filename
	// /return new Model, otherwhise return null;
	Model * load(const std::string & filename);
	//std::vector<Mesh *> _meshes;

private:

	
	

	int parseFaceType(const std::string & str);
	bool parseTexture(Texture * texture, std::vector<std::string> & tokens);

	void readFaces(const std::vector<std::string> & tokens, std::vector<Face> & faces);
	//void readUsemtl(const std::vector<std::string> & tokens, std::map<std::string, int> & materialsMap, int & materialID);
	bool exportFaces(Model * model, std::vector<Face> & faces, const std::vector<vec3> & positions, const std::vector<vec3> & normals, const std::vector<vec2> & texcoord, const int & materialID, const std::string & name);
	void triangulate(std::vector<VertexIndex> & face, const std::vector<vec3> & positions, std::vector<unsigned int> & outIndices);
	void vertexTriangulate(std::vector<VertexIndex> & face, const std::vector<vec3> & positions, std::vector<VertexIndex> & newFaces);
	void arrangeIndices(std::vector<int> & indices, int newIndice);


	bool readMaterial(const std::string & baseDir, const std::string & materialName, std::vector<Material> & materials, std::map<std::string,int> & materialMap);
	void loadMtl(std::istream & stream, std::map<std::string, int> & materialMap, std::vector<Material> & materials);

};


#endif

// TODO 
// use VertexIndex instead for every component of face a std::vector
/*struct Face
{
bool hasNormals;
bool hasTexture;



std::vector<int> indexes;
std::vector<int> texIndexes;
std::vector<int> normalIndexes;

Face()
{

hasTexture = false;
hasNormals = false;
}

/*void set(int a, int b, int c, int d,int * t, int * n)
{
/*indices[0] = a;
indices[1] = b;
indices[2] = c;
indices[3] = d;

texIndices[0] = t[0];
texIndices[1] = t[1];
texIndices[2] = t[2];
texIndices[3] = t[3];

normalIndices[0] = n[0];
normalIndices[1] = n[1];
normalIndices[2] = n[2];
normalIndices[3] = n[3];

}
};*/