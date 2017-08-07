
#ifndef _MESHLOADER_H_
#define _MESHLOADER_H_


#include <string>
#include "utils/stringutils.h"
#include "objloader.h"
#include "mesh.h"


class MeshLoader {

private:

	MeshLoader() {}
	~MeshLoader() {}

	// TODO
	// impove to static class
	ObjLoader obj;


public:

	static MeshLoader & getInstance() 
	{
		static MeshLoader instance;

		return instance;
	}





	// GEO and OBJ are accepted
	bool loadMesh(const std::string & filename, std::vector<Mesh*> & meshes, const int & bvh = 0) 
	{
		//mesh = nullptr;
		std::string extension = StringUtils::fileExtension(filename);
		extension = StringUtils::toLower(extension);

		if (extension == "obj")
		{
			ObjLoader::Model * model = obj.load(filename);

			if (model != nullptr) {
				
				// TODO
				// improve this parser
				// parse obj loader model to our mesh
				for (size_t i = 0; i < model->meshes.size(); i++)
				{
					Mesh * mesh = new Mesh(model->meshes[i]->indices, model->meshes[i]->vertices, model->meshes[i]->normals, model->meshes[i]->texcoord,bvh);

					if (model->materials.size() > 0 && model->meshes[i]->materialID != -1) {

						

						const ObjLoader::Material & m = model->materials[model->meshes[i]->materialID];

						Material * mat = new Material();

						mat->setProperty(MAT_DIFFUSE, m.kd);
						mat->setProperty(MAT_SPECULAR, m.ks);
						mat->setProperty(MAT_AMBIENT, m.ka);
						mat->setProperty(MAT_EMISSION, m.ke);
						mat->setProperty(MAT_TRANSMITTANCE, m.kt);

						mat->setProperty(MAT_SHININESS, m.ns);
						mat->setProperty(MAT_TRANSPARENCY, m.d);
						mat->setProperty(MAT_REFRACTION, m.ni);
						mat->setProperty(MAT_ILLUM, m.illum);

						if (!m.map_kd.name.empty())
						{
							Texture * texture = new Texture;
							if (texture->load(model->path + m.map_kd.name)) {
								mat->setTexture(texture);
							}
							else
								delete texture;
						}


						mesh->setMaterial(mat);

					}

					meshes.push_back(mesh);
				}

				//std::cout << obj._meshes[0]->_vertices.size() << std::endl;
				return true;
			}
			else 
				return false;
				

		}
		else 
		{
			std::cerr << "MeshLoader: Unsupoorted file: " << filename << "." << std::endl;
			return false;
		}
	}

};

#endif