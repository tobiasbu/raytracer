
#ifndef _IMAGEOPERATORS_H_
#define _IMAGEOPERATORS_H_

#include "image.h"


namespace ImageOperator
{
	void swap();

	void clamp(Image * src, int min = 0, int max = 255);

	// absolute
	void abs(Image * src);

	// absolute difference
	void absdiff(Image * src, Image * src2, Image * dst);

	// subtract pixels
	void sub(Image * src, Image * src2, Image * dst, bool clamp = false);

	// add pixels from add to src, alpha is ignored
	void add(Image * src, Image * src2, bool clamp = false);

	// multiply pixels to source from dest
	void mul(Image * src, Image * src2, Image * dst);

	void bitwise_left_shift(Image * src, Image * src2);

	void bitwise_left_shift(Image * src, Image * mask, const Color & color);

	void bitwise_and(Image * src, Image * src2);

	void bitwise_and(Image * src, Image * mask, const Color & color);

	void bitwise_xor(Image * src, Image * src2);

	void bitwise_xor(Image * src, Image * mask, const Color & color);

	void bitwise_or(Image * src, Image * src2);

	void bitwise_or(Image * src, Image * src2, const Color & color);

	void bitwise_not(Image * src, Image * src2);

	void minMaxLuminance(const Image * src, float & min, float & max, float minRange = 0, float maxRange = 255);

	// extract colors RGB between min and max luminance (HSV value brightness) 
	void extractLuminance(Image * src, Image * dst, float min, float max);

	// extract colors RGB between min and max saturaion (HSV) 
	void extractSat(Image * src, Image * dst, float min, float max);

	void swapBinaryColor(Image * src, const Color & color);

	void bitwise_or_mask(Image * src, Image * mask, const Color & color = Color::white);
}

#endif