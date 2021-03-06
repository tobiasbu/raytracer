
#ifndef   _TEXTURE_H_
#define   _TEXTURE_H_

#include "image/image.h"
#include "math/vec2.h"

enum TextureInterpolation {
	Point,
	Billinear
	// Trillinear TODO
};

enum TextureWrap {
	Clamp,
	Repeat
	// TODO mirror
};

class Texture 
{
private:

	TextureInterpolation _interpolation = TextureInterpolation::Point;
	TextureWrap _wrap = TextureWrap::Clamp;
	Image * _image;
	bool _loaded;

	unsigned char * getPixelAt(int x, int y);
	void getPixel(int x, int y, unsigned char *);
	Color getPixelColor(int x, int y);

public:

	Texture();
	~Texture();
	bool load(const std::string & filename);
	bool loaded();
	void setInterpolation(const TextureInterpolation & interpolation);
	void setWrap(const TextureWrap & wrap);
	
	Color getTexel(float x, float y);
	Color getTexel(vec2 texCoord);

};

#endif