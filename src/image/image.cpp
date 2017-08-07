
#include "image/image.h"

#include "image/imageloader.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <time.h>
#include <string.h>
#include <iostream>

Image::Image() 
	:
	_width(0),
	_height(0),
	_bufferSize(0)
{

}

Image::Image(const Image & img)
{

	if (img._pixels) {

		
		this->_pixels = new unsigned char[img._bufferSize];
		memcpy(&this->_pixels[0], img._pixels, img._bufferSize);
		this->_width = img._width;
		this->_height = img._height;
		this->_bufferSize = img._bufferSize;
	}
}

Image::Image(unsigned int width, unsigned int height, const Color & fillColor) {
	
	this->create(width, height, fillColor);

}

Image::~Image()
{
	if (_pixels || _pixels != NULL || _pixels != nullptr)
		delete _pixels;
}

void Image::create(unsigned int width, unsigned int height, const Color & fillColor) {

	_width = width;
	_height = height;


	_bufferSize = _width * _height * 4;
	_pixels = new unsigned char[_bufferSize];

	
	unsigned char * rgba = fillColor.toColor32();

	//for (unsigned char * it = &_pixels[0]; it != &_pixels[sizeof(_pixels)]; it += 4)
	//{
	//	*it = 0;
	//}

	/**/
	
	unsigned char * ptr = &_pixels[0];
	unsigned char * end = &_pixels[_bufferSize -1];

	while (ptr < end)
	{
		*ptr++ = rgba[0];
		*ptr++ = rgba[1];
		*ptr++ = rgba[2];
		*ptr++ = rgba[3];
	}
}

void Image::setPixel(unsigned int x, unsigned int y, const Color & color) {

	unsigned char * rgba = color.toColor32();

	setPixel(x, y, rgba[0], rgba[1], rgba[2], rgba[3]);
}

void Image::setPixel(unsigned int x, unsigned int y, const unsigned char * color)
{
	int index = (x + y * _width) * 4;

	_pixels[index] = color[0];
	_pixels[index + 1] = color[1];
	_pixels[index + 2] = color[2];
	_pixels[index + 3] = color[3];
}

void Image::setPixel(unsigned int x, unsigned int y, int r, int g, int b, int a) 
{

	int index = (x + y * _width) * 4;

	_pixels[index] = r;
	_pixels[index + 1] = g;
	_pixels[index + 2] = b;
	_pixels[index + 3] = a;

	// can used pointers
}

const unsigned char * Image::getPixelsPtr() const
{
	return &_pixels[0];
}

unsigned char * Image::getPixels() const
{
	return _pixels;
}

Color Image::getPixelColor(unsigned int x, unsigned int y) const
{
	// this is row order
	// x + y * _width * 4
	//  mistake! this is not the right
	//  y * _height * 4 + x


	int index = x + y * _width;

	const unsigned char* pixel = &_pixels[index * 4];
	return Color((float)pixel[0] / 255.f, (float)pixel[1] / 255.f, (float)pixel[2] / 255.f, (float)pixel[3] / 255.f);
}

unsigned char * Image::getPixel(unsigned int x, unsigned int y) const
{

	int index = (x + y * _width) * 4;

	unsigned char * p = new unsigned char[4];
	p[0] = _pixels[index];
	p[1] = _pixels[index+1];
	p[2] = _pixels[index+2];
	p[3] = _pixels[index+3];
	return p;
}

unsigned int Image::getWidth() const
{
	return _width;
}

unsigned int Image::getHeight() const
{
	return _height;
}


bool Image::save(const std::string & filename) const
{
	if (_pixels)
	{
		if (!ImageLoader::getInstance().saveImageFile(filename, _pixels, (int&)_width, (int&)_height))
		{
			//std::cout << "Failed to save image file: " << filename << std::endl;
			return false;
		}

		return true;

	} else
		return false;
	

}

bool Image::load(const std::string & filename)
{
	if (!ImageLoader::getInstance().loadImage(filename, &_pixels, (int&)_width, (int&)_height))
	{
		//_pixels = nullptr;
		//std::cout << "Failed to load image file: " << filename << std::endl;
		return false;
	}
	else
		return true;
}

size_t Image::getBufferSize() const
{
	return _bufferSize;
}

Image * Image::clone() const
{
	if (_width <= 0 || _height <= 0)
		return NULL;

	Image * copy = new Image(*this);

	return copy;
}

void Image::clear(const Color & color)
{
	if (_width <= 0 && _height <= 0)
		return;

	unsigned char * rgba = color.toColor32();

	unsigned char * ptr = &_pixels[0];
	unsigned char * end = &_pixels[_bufferSize - 1];

	while (ptr < end)
	{
		*ptr++ = rgba[0];
		*ptr++ = rgba[1];
		*ptr++ = rgba[2];
		*ptr++ = rgba[3];
	}
}

void Image::resize(unsigned int width, unsigned int height, int sample)
{
	if (width < 0 || height < 0)
		return;

	unsigned char* newData = new unsigned char[width * height * 4];

	double scaleWidth = (double)width / (double)_width;
	double scaleHeight = (double)height / (double)_height;

	int cy = 0, cx = 0;

	for (; cy < height; cy++)
	{
		for (cx = 0; cx < width; cx++)
		{
			//int pixel = (cx + cy * width) * 4;
			int pixel = (cy * (width * 4)) + (cx * 4);
			//int pixel = (cy * (width * 4)) + (cx * 4);

			if (sample == 0)
			{
				//int nearestMatch = (((int)(cy / scaleHeight) * (_width * 4)) + ((int)(cx / scaleWidth) * 4));

				//int nearestMatch = (int)((cx / scaleWidth) * 4) + ((int)(cy / scaleHeight) * (_width * 4));
				int nearestMatch = (((int)(cy / scaleHeight) * (_width * 4)) + ((int)(cx / scaleWidth) * 4));


				newData[pixel] = _pixels[nearestMatch];
				newData[pixel + 1] = _pixels[nearestMatch + 1];
				newData[pixel + 2] = _pixels[nearestMatch + 2];
				newData[pixel + 3] = _pixels[nearestMatch + 3];
			}
		}

	}

	delete[] _pixels;
	_pixels = newData;
	_width = width;
	_height = height;
	_bufferSize = width * height * 4;
}

// old method
/*bool Image::saveBMP(const char * filename) {

	// default dpi
	int dpi = 72;


	//FILE *f;

	int pixel_size_bytes = _width * _height * 3;
	int filesize = 54 + pixel_size_bytes;

	double factor = 39.375;
	int m = static_cast<int>(factor);

	int ppm = dpi*m;

	unsigned char bmpfileheader[14] = { 'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0 };
	unsigned char bmpinfoheader[40] = { 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0 };

	bmpfileheader[2] = (unsigned char)(filesize);
	bmpfileheader[3] = (unsigned char)(filesize >> 8);
	bmpfileheader[4] = (unsigned char)(filesize >> 16);
	bmpfileheader[5] = (unsigned char)(filesize >> 24);

	bmpinfoheader[4] = (unsigned char)(_width);
	bmpinfoheader[5] = (unsigned char)(_width >> 8);
	bmpinfoheader[6] = (unsigned char)(_width >> 16);
	bmpinfoheader[7] = (unsigned char)(_width >> 24);

	bmpinfoheader[8] = (unsigned char)(_height);
	bmpinfoheader[9] = (unsigned char)(_height >> 8);
	bmpinfoheader[10] = (unsigned char)(_height >> 16);
	bmpinfoheader[11] = (unsigned char)(_height >> 24);

	bmpinfoheader[21] = (unsigned char)(pixel_size_bytes);
	bmpinfoheader[22] = (unsigned char)(pixel_size_bytes >> 8);
	bmpinfoheader[23] = (unsigned char)(pixel_size_bytes >> 16);
	bmpinfoheader[24] = (unsigned char)(pixel_size_bytes >> 24);

	bmpinfoheader[25] = (unsigned char)(ppm);
	bmpinfoheader[26] = (unsigned char)(ppm >> 8);
	bmpinfoheader[27] = (unsigned char)(ppm >> 16);
	bmpinfoheader[28] = (unsigned char)(ppm >> 24);

	bmpinfoheader[29] = (unsigned char)(ppm);
	bmpinfoheader[30] = (unsigned char)(ppm >> 8);
	bmpinfoheader[31] = (unsigned char)(ppm >> 16);
	bmpinfoheader[32] = (unsigned char)(ppm >> 24);

	//f = fopen_s(filename, "wb");

	FILE * file;
	errno_t err;

	if ((err = fopen_s(&file, filename, "wb")) != 0) {

		fprintf(stderr, "ERROR: could not save *.BMP image file: %s \n", filename);
		return false;
	}

	fwrite(bmpfileheader, 1, 14, file);
	fwrite(bmpinfoheader, 1, 40, file);

	for (int i = 0; i < _bufferSize; i += 4) {
		//for (int x = 0; x < _width; x++) {

		//int index = (x + y * _width) * 4;

		//int * rgb = _pixels[i].getRGB();

		//double red = (data[i].r) * 255;
		//double green = (data[i].g) * 255;
		//double blue = (data[i].b) * 255;

		unsigned char color[3] = { _pixels[i + 2], _pixels[i + 1], _pixels[i] };

		fwrite(color, 1, 3, file);
		//}
	}

	fclose(file);

	return true;

}*/