
#ifndef _IMAGE_H_
#define _IMAGE_H_


/*#ifdef _WIN32 
#define _CRT_SECURE_NO_DEPRECATE
#endif*/

#include <string>
#include "render/color.h"

class Image {

private:

	

	unsigned int _width;
	unsigned int _height;
	unsigned char * _pixels;
	//Color * _pixels;
	size_t _bufferSize;

	Image(const Image &);

public:
	
	Image();
	~Image();
	Image(unsigned int width, unsigned int height, const Color & fillColor = Color::black);

	void create(unsigned int width, unsigned int height, const Color & fillColor = Color::black);

	bool load(const std::string & filename);
	bool save(const std::string & filename) const;

	void setPixel(unsigned int x, unsigned int y, const Color & color);
	void setPixel(unsigned int x, unsigned int y, const unsigned char * color);
	void setPixel(unsigned int x, unsigned int y, int r, int g, int b, int a = 255);
	
	Color getPixelColor(unsigned int x, unsigned int y) const;
	unsigned char * getPixel(unsigned int x, unsigned int y) const;
	const unsigned char * getPixelsPtr() const;
	unsigned char * getPixels() const;
	unsigned int getWidth() const;
	unsigned int getHeight() const;
	size_t getBufferSize() const;

	Image * clone() const;

	void clear(const Color & color = Color::black);

	void resize(unsigned int width, unsigned int height, int sample = 0);
	
	// old method
	//bool saveBMP(const char * filename);

};

#endif