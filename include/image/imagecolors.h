
#ifndef _IMAGECOLORS_H_
#define _IMAGECOLORS_H_

#include "image.h"
#include "math/maths.h"



namespace ImageColors {


	enum GrayscaleType 
	{
		Average,
		Luminance,
		Desaturation,
		MaxDecomposition,
		MinDecomposition,
		RedChannel,
		BlueChannel,
		GreenChannel,
		Luma,
		HSP
	};

	// transform image colors to grayscale
	bool grayscale(Image * image, const GrayscaleType & method = GrayscaleType::Average);

	// posterize image
	// level beteween 2 and 256
	bool posterize(Image * image, int level = 3);


	// range -255...+255 
	bool contrastBrightnes(Image * image, float c, float b);


	bool threshold(Image * image, int level = 127);

	// check https://github.com/rmtheis/bradley-adaptive-thresholding/blob/master/main.cpp
	bool adaptiveThreshold(Image * src, Image * dst, int blockSize, int level = 255, int delta = 2);

	// negate image colors
	bool invert(Image * image);

	

	bool colorize(Image * image,const float & h, const float & s, const float & v);



	unsigned char * HSVtoRGB(float & h, float & s, float & v);

	void RGBtoHSV(unsigned char * in, float & h, float & s, float & v);

	void antialiasing(Image * image, int samples = 3);

	// blend over RGB colors
	void blend(Image * src, Image * src2, bool inversedBlend = false);

	

};

#endif