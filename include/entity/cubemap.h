
#ifndef _CUBEMAP_H_
#define _CUBEMAP_H_

#include "render/texture.h"

class CubeMap {

private:


	Texture * _textures[6];

public:

	enum CubeMapFace {
		Left = 0,
		Right,
		Top,
		Bottom,
		Front,
		Back,
	};


	CubeMap() 
	{
		for (int i = 0; i < 6; i++)
			_textures[i] = nullptr;
	}

	// Loads CubeMap.
	// Set directory, filename suffix and files extension.
	// /param filespath = path/to/cubemap/
	// /param suffix = cubemap_ (front, left...);
	// /param extension = file format (bmp, png...)
	// /return true if all textures was sucefully loaded
	bool load(const std::string & filespath, const std::string & fileSuffix, std::string & extension);

	void setTexture(const CubeMapFace & face, Texture * texture);

	bool getTexel(const vec3 & direction, Color & outColor);

	static vec2 getTexelDirection(const vec3 & direction, int & faceIndex);

};

#endif