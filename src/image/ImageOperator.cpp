
#include "image/ImageOperators.h"
#include "math/maths.h"
#include "image/imagecolors.h"

namespace ImageOperator
{


	void clamp(Image * src, int min, int max)
	{
		if (!src)
			return;

		if (src->getWidth() <= 0 || src->getHeight() <= 0)
			return;


		unsigned char * ptr = &src->getPixels()[0];
		unsigned char * end = &src->getPixels()[src->getBufferSize() - 1];

		while (ptr < end)
		{
			*ptr++ = Math::clamp(*ptr, min, max);
			*ptr++ = Math::clamp(*ptr, min, max);
			*ptr++ = Math::clamp(*ptr, min, max);

			// ignore alpha
			ptr++;
		}
	}

	// absolute
	void abs(Image * src)
	{
		if (!src)
			return;

		if (src->getWidth() <= 0 || src->getHeight() <= 0)
			return;


		unsigned char * ptr = &src->getPixels()[0];
		unsigned char * end = &src->getPixels()[src->getBufferSize() - 1];

		while (ptr < end)
		{
			*ptr++ = std::abs(*ptr);
			*ptr++ = std::abs(*ptr);
			*ptr++ = std::abs(*ptr);

			// ignore alpha
			ptr++;
		}
	}

	// absolute difference
	void absdiff(Image * src, Image * src2, Image * dst)
	{
		if (!src || !src2)
			return;

		if (src->getWidth() <= 0 || src->getHeight() <= 0)
			return;

		if (src2->getWidth() <= 0 || src2->getHeight() <= 0)
			return;

		if (src2->getWidth() != src->getWidth() || src2->getHeight() != src->getHeight())
			return;

		unsigned char * ptr = &src->getPixels()[0];
		unsigned char * end = &src->getPixels()[src->getBufferSize() - 1];
		unsigned char * ptr2 = &src2->getPixels()[0];
		unsigned char * pdst = &dst->getPixels()[0];



		while (ptr < end)
		{
			*pdst++ = std::abs(*ptr++ - *ptr2++);
			*pdst++ = std::abs(*ptr++ - *ptr2++);
			*pdst++ = std::abs(*ptr++ - *ptr2++);

			// ignore alpha
			pdst++;
			ptr++;
			ptr2++;
		}
	}

	// subtract pixels
	void sub(Image * src, Image * src2, Image * dst, bool clamp)
	{
		if (!src || !src2)
			return;

		if (src->getWidth() <= 0 || src->getHeight() <= 0)
			return;

		if (src2->getWidth() <= 0 || src2->getHeight() <= 0)
			return;

		if (src2->getWidth() != src->getWidth() || src2->getHeight() != src->getHeight())
			return;

		unsigned char * ptr = &src->getPixels()[0];
		unsigned char * ptr2 = &src2->getPixels()[0];
		unsigned char * end = &src->getPixels()[src->getBufferSize() - 1];
		unsigned char * dptr = &dst->getPixels()[0];


		while (ptr < end)
		{
			if (clamp) {
				*dptr++ = Math::clamp(*ptr++ - *ptr2++, 0, 255);
				*dptr++ = Math::clamp(*ptr++ - *ptr2++, 0, 255);
				*dptr++ = Math::clamp(*ptr++ - *ptr2++, 0, 255);
			}
			else
			{
				*dptr++ = *ptr++ - *ptr2++;
				*dptr++ = *ptr++ - *ptr2++;
				*dptr++ = *ptr++ - *ptr2++;

			}

			// ignore alpha
			ptr++;
			dptr++;
			ptr2++;
		}
	}

	// add pixels from add to src, alpha is ignored
	void add(Image * src, Image * src2, bool clamp)
	{
		if (!src || !src2)
			return;

		if (src->getWidth() <= 0 || src->getHeight() <= 0)
			return;

		if (src2->getWidth() <= 0 || src2->getHeight() <= 0)
			return;

		if (src2->getWidth() != src->getWidth() || src2->getHeight() != src->getHeight())
			return;

		unsigned char * ptr = &src->getPixels()[0];
		unsigned char * end = &src->getPixels()[src->getBufferSize() - 1];
		unsigned char * dptr = &src2->getPixels()[0];


		while (ptr < end)
		{
			if (clamp) {
				*ptr++ = Math::clamp((float)*ptr + (float)*dptr++, 0, 255);
				*ptr++ = Math::clamp((float)*ptr + (float)*dptr++, 0, 255);
				*ptr++ = Math::clamp((float)*ptr + (float)*dptr++, 0, 255);
			}
			else
			{
				*ptr++ = *ptr + *dptr++;
				*ptr++ = *ptr + *dptr++;
				*ptr++ = *ptr + *dptr++;
			}
			ptr++;
			dptr++;
		}

		/*for (size_t y = 0; y < src->getHeight(); ++y)
		{
		for (size_t x = 0; x < src->getWidth(); ++x)
		{
		unsigned char * col = src->getPixel(x, y);
		const unsigned char * coladd = add->getPixel(x, y);

		for (size_t k = 0; k < 3; ++k)
		{
		col[k] = (unsigned char)Math::clampint(col[k] + coladd[k], 0, 255);
		}

		src->setPixel(x, y, col);

		}

		}*/

	}

	// multiply pixels to source from dest
	void mul(Image * src, Image * src2, Image * dst)
	{
		if (!src || !dst)
			return;

		if (src->getWidth() <= 0 || src->getHeight() <= 0)
			return;

		if (dst->getWidth() <= 0 || dst->getHeight() <= 0)
			return;

		if (dst->getWidth() != src->getWidth() || dst->getHeight() != src->getHeight())
			return;

		unsigned char * ptr = &src->getPixels()[0];
		unsigned char * ptr2 = &src2->getPixels()[0];
		unsigned char * end = &src->getPixels()[src->getBufferSize() - 1];
		unsigned char * dptr = &dst->getPixels()[0];

		while (ptr < end)
		{
			*dptr++ = *ptr++ * *ptr2++;
			*dptr++ = *ptr++ * *ptr2++;
			*dptr++ = *ptr++ * *ptr2++;

			// ignore alpha
			ptr++;
			dptr++;
			ptr2++;
		}

		/*for (size_t y = 0; y < src->getHeight(); ++y)
		{
		for (size_t x = 0; x < src->getWidth(); ++x)
		{
		unsigned char * a = src->getPixel(x, y);
		const unsigned char * b = dst->getPixel(x, y);

		for (size_t k = 0; k < 3; ++k)
		{
		a[k] = (int)a[k] * (int)b[k];
		}

		src->setPixel(x, y, a);

		}

		}*/

	}


	void bitwise_left_shift(Image * src, Image * src2)
	{
		if (!src || !src2)
			return;

		if (src->getWidth() <= 0 || src->getHeight() <= 0)
			return;

		if (src->getWidth() <= 0 || src2->getHeight() <= 0)
			return;

		if (src2->getWidth() != src->getWidth() || src2->getHeight() != src->getHeight())
			return;


		unsigned char * ptr = &src->getPixels()[0];
		unsigned char * dptr = &src2->getPixels()[0];
		unsigned char * end = &src->getPixels()[src->getBufferSize() - 1];

		while (ptr < end)
		{

			*ptr++ = *ptr << *dptr++;
			*ptr++ = *ptr << *dptr++;
			*ptr++ = *ptr << *dptr++;
			ptr++;
			dptr++;
		}

		/*for (size_t y = 0; y < src->getHeight(); ++y)
		{
		for (size_t x = 0; x < src->getWidth(); ++x)
		{
		unsigned char * a = src->getPixel(x, y);
		const unsigned char * b = dst->getPixel(x, y);

		for (size_t k = 0; k < 3; ++k)
		{
		a[k] = a[k] << b[k];
		}

		src->setPixel(x, y, a);

		}

		}*/

	}

	void bitwise_left_shift(Image * src, Image * mask, const Color & color)
	{
		if (!src || !mask)
			return;

		if (src->getWidth() <= 0 || src->getHeight() <= 0)
			return;

		if (mask->getWidth() <= 0 || mask->getHeight() <= 0)
			return;

		if (mask->getWidth() != src->getWidth() || mask->getHeight() != src->getHeight())
			return;


		unsigned char * ptr = &src->getPixels()[0];
		unsigned char * end = &src->getPixels()[src->getBufferSize() - 1];
		unsigned char * dptr = &mask->getPixels()[0];
		unsigned char * c = color.toColor32();

		while (ptr < end)
		{

			if (*dptr != 0)
			{
				*ptr++ = *ptr & c[0];
				*ptr++ = *ptr & c[1];
				*ptr++ = *ptr & c[2];
				ptr++;
			}
			else
				ptr += 4;

			dptr += 4;
		}


	}

	void bitwise_and(Image * src, Image * src2)
	{
		if (!src || !src2)
			return;

		if (src->getWidth() <= 0 || src->getHeight() <= 0)
			return;

		if (src->getWidth() <= 0 || src2->getHeight() <= 0)
			return;

		if (src2->getWidth() != src->getWidth() || src2->getHeight() != src->getHeight())
			return;



		unsigned char * ptr = &src->getPixels()[0];
		unsigned char * dptr = &src2->getPixels()[0];
		unsigned char * end = &src->getPixels()[src->getBufferSize() - 1];


		while (ptr < end)
		{

			*ptr++ = *ptr & *dptr++;
			*ptr++ = *ptr & *dptr++;
			*ptr++ = *ptr & *dptr++;
			ptr++;
			dptr++;
		}



		/*for (size_t y = 0; y < src->getHeight(); ++y)
		{
		for (size_t x = 0; x < src->getWidth(); ++x)
		{
		unsigned char * a = src->getPixel(x, y);
		const unsigned char * b = src2->getPixel(x, y);

		for (size_t k = 0; k < 3; ++k)
		{
		a[k] = a[k] & b[k];
		}

		src->setPixel(x, y, a);

		}

		}*/

	}

	void bitwise_and(Image * src, Image * mask, const Color & color)
	{
		if (!src || !mask)
			return;

		if (src->getWidth() <= 0 || src->getHeight() <= 0)
			return;

		if (mask->getWidth() <= 0 || mask->getHeight() <= 0)
			return;

		if (mask->getWidth() != src->getWidth() || mask->getHeight() != src->getHeight())
			return;



		unsigned char * ptr = &src->getPixels()[0];
		unsigned char * end = &src->getPixels()[src->getBufferSize() - 1];
		unsigned char * dptr = &mask->getPixels()[0];
		unsigned char * c = color.toColor32();

		while (ptr < end)
		{

			if (*dptr != 0)
			{
				*ptr++ = *ptr & c[0];
				*ptr++ = *ptr & c[1];
				*ptr++ = *ptr & c[2];
				ptr++;
			}
			else
				ptr += 4;

			dptr += 4;
		}

		//delete c;

		/*for (size_t y = 0; y < src->getHeight(); ++y)
		{
		for (size_t x = 0; x < src->getWidth(); ++x)
		{
		unsigned char * a = src->getPixel(x, y);
		const unsigned char * b = src2->getPixel(x, y);

		for (size_t k = 0; k < 3; ++k)
		{
		a[k] = a[k] & b[k];
		}

		src->setPixel(x, y, a);

		}

		}*/

	}

	void bitwise_xor(Image * src, Image * src2)
	{
		if (!src || !src2)
			return;

		if (src->getWidth() <= 0 || src->getHeight() <= 0)
			return;

		if (src2->getWidth() <= 0 || src2->getHeight() <= 0)
			return;

		if (src2->getWidth() != src->getWidth() || src2->getHeight() != src->getHeight())
			return;

		unsigned char * ptr = &src->getPixels()[0];
		unsigned char * end = &src->getPixels()[src->getBufferSize() - 1];
		unsigned char * dptr = &src2->getPixels()[0];

		while (ptr < end)
		{

			*ptr++ = *ptr ^ *dptr++;
			*ptr++ = *ptr ^ *dptr++;
			*ptr++ = *ptr ^ *dptr++;
			ptr++;
			dptr++;
		}

		/*for (size_t y = 0; y < src->getHeight(); ++y)
		{
		for (size_t x = 0; x < src->getWidth(); ++x)
		{
		unsigned char * a = src->getPixel(x, y);
		const unsigned char * b = dst->getPixel(x, y);

		for (size_t k = 0; k < 3; ++k)
		{
		a[k] = a[k] ^ b[k];
		}

		src->setPixel(x, y, a);

		}

		}*/

	}

	void bitwise_xor(Image * src, Image * mask, const Color & color)
	{
		if (!src || !mask)
			return;

		if (src->getWidth() <= 0 || src->getHeight() <= 0)
			return;

		if (mask->getWidth() <= 0 || mask->getHeight() <= 0)
			return;

		if (mask->getWidth() != src->getWidth() || mask->getHeight() != src->getHeight())
			return;



		unsigned char * ptr = &src->getPixels()[0];
		unsigned char * end = &src->getPixels()[src->getBufferSize() - 1];
		unsigned char * dptr = &mask->getPixels()[0];
		unsigned char * c = color.toColor32();

		while (ptr < end)
		{

			if (*dptr != 0)
			{
				*ptr++ = *ptr ^ c[0];
				*ptr++ = *ptr ^ c[1];
				*ptr++ = *ptr ^ c[2];
				ptr++;
			}
			else
				ptr += 4;

			dptr += 4;
		}


	}

	void bitwise_or(Image * src, Image * src2)
	{
		if (!src || !src2)
			return;

		if (src->getWidth() <= 0 || src->getHeight() <= 0)
			return;

		if (src2->getWidth() <= 0 || src2->getHeight() <= 0)
			return;

		if (src2->getWidth() != src->getWidth() || src2->getHeight() != src->getHeight())
			return;

		unsigned char * ptr = &src->getPixels()[0];
		unsigned char * end = &src->getPixels()[src->getBufferSize() - 1];
		unsigned char * dptr = &src2->getPixels()[0];

		while (ptr < end)
		{

			*ptr++ = *ptr | *dptr++;
			*ptr++ = *ptr | *dptr++;
			*ptr++ = *ptr | *dptr++;
			ptr++;
			dptr++;
		}

		/*for (size_t y = 0; y < src->getHeight(); ++y)
		{
		for (size_t x = 0; x < src->getWidth(); ++x)
		{
		unsigned char * a = src->getPixel(x, y);
		const unsigned char * b = dst->getPixel(x, y);

		for (size_t k = 0; k < 3; ++k)
		{
		a[k] = a[k] | b[k];
		}

		src->setPixel(x, y, a);

		}

		}

		}*/
	}

	void bitwise_or(Image * src, Image * src2, const Color & color)
	{
		if (!src || !src2)
			return;

		if (src->getWidth() <= 0 || src->getHeight() <= 0)
			return;

		if (src2->getWidth() <= 0 || src2->getHeight() <= 0)
			return;

		if (src2->getWidth() != src->getWidth() || src2->getHeight() != src->getHeight())
			return;

		unsigned char * ptr = &src->getPixels()[0];
		unsigned char * end = &src->getPixels()[src->getBufferSize() - 1];
		unsigned char * dptr = &src2->getPixels()[0];
		unsigned char * c = color.toColor32();

		while (ptr < end)
		{

			if (*dptr != 0)
			{
				*ptr++ = *ptr | c[0];
				*ptr++ = *ptr | c[1];
				*ptr++ = *ptr | c[2];
				ptr++;
			}
			else
				ptr += 4;

			dptr += 4;
		}


	}

	void bitwise_not(Image * src, Image * src2)
	{
		if (!src || !src2)
			return;

		if (src->getWidth() <= 0 || src->getHeight() <= 0)
			return;

		if (src2->getWidth() <= 0 || src2->getHeight() <= 0)
			return;

		if (src2->getWidth() != src->getWidth() || src2->getHeight() != src->getHeight())
			return;

		unsigned char * ptr = &src->getPixels()[0];
		unsigned char * end = &src->getPixels()[src->getBufferSize() - 1];
		unsigned char * dptr = &src2->getPixels()[0];

		while (ptr < end)
		{

			*ptr++ = ~*dptr++;
			*ptr++ = ~*dptr++;
			*ptr++ = ~*dptr++;
			ptr++;
			dptr++;
		}
	}

	void minMaxLuminance(const Image * src, float & min, float & max, float minRange, float maxRange)
	{
		if (!src)
			return;

		if (src->getWidth() <= 0 || src->getHeight() <= 0)
			return;

		/*min = 0;
		max = 0;

		if (minRange < 0)
		minRange = 0;

		if (maxRange > 255)
		maxRange = 255;*/

		if (min < 0)
			min = 0;
		if (max > 255)
			max = 255;


		unsigned char * ptr = &src->getPixels()[0];
		unsigned char * end = &src->getPixels()[src->getBufferSize() - 1];

		while (ptr < end)
		{

			if ((float)*ptr > max && *ptr <= maxRange)
				max = *ptr;

			if ((float)*ptr < min && *ptr >= minRange)
				min = *ptr;

			ptr += 4;

		}
	}

	// extract colors RGB between min and max luminance (HSV value brightness)
	void extractLuminance(Image * src, Image * dst, float min, float max)
	{
		if (!src || !dst)
			return;

		if (src->getWidth() <= 0 || src->getHeight() <= 0)
			return;

		
		unsigned char * ptr = &src->getPixels()[0];
		unsigned char * end = &src->getPixels()[src->getBufferSize() - 1];

		unsigned char * dptr = &dst->getPixels()[0];
		float h, s, v;

		while (ptr < end)
		{
			ImageColors::RGBtoHSV(&*ptr, h, s, v);

			//v *= 255;

			//std::cout << v << std::endl;

			if (v >= min && v <= max)
			{ 
				*dptr++ = *ptr++;
				*dptr++ = *ptr++;
				*dptr++ = *ptr++;
				dptr++;
				ptr++;
			}
			else
			{
				*dptr++ = 0;
				*dptr++ = 0;
				*dptr++ = 0;
				dptr++;
				ptr += 4;
			
			}
		}


	}

	void extractSat(Image * src, Image * dst, float min, float max)
	{
		if (!src || !dst)
			return;

		if (src->getWidth() <= 0 || src->getHeight() <= 0)
			return;

		unsigned char * ptr = &src->getPixels()[0];
		unsigned char * end = &src->getPixels()[src->getBufferSize() - 1];

		unsigned char * dptr = &dst->getPixels()[0];
		float h, s, v;

		while (ptr < end)
		{
			ImageColors::RGBtoHSV(&*ptr, h, s, v);

			s *= 255;

			//std::cout << s << std::endl;

			if (s > min && s <= max)
			{
				*dptr++ = *ptr++;
				*dptr++ = *ptr++;
				*dptr++ = *ptr++;
				dptr++;
				ptr++;
			}
			else
			{
				dptr += 4;
				ptr += 4;

			}
		}


	}

	void swapBinaryColor(Image * src, const Color & color)
	{
		if (!src)
			return;

		if (src->getWidth() <= 0 || src->getHeight() <= 0)
			return;


		unsigned char * c = color.toColor32();
		unsigned char * ptr = &src->getPixels()[0];
		unsigned char * end = &src->getPixels()[src->getBufferSize() - 1];

		while (ptr < end)
		{

			if (*ptr != 0) {
				*ptr++ = c[0];
				*ptr++ = c[1];
				*ptr++ = c[2];
				ptr++;
			}
			else
				ptr += 4;

		}
	}

	void bitwise_or_mask(Image * src, Image * mask, const Color & color)
	{
		if (!src || !mask)
			return;

		if (src->getWidth() <= 0 || src->getHeight() <= 0)
			return;

		if (mask->getWidth() <= 0 || mask->getHeight() <= 0)
			return;

		if (mask->getWidth() != src->getWidth() || mask->getHeight() != src->getHeight())
			return;

		unsigned char * ptr = &src->getPixels()[0];
		unsigned char * end = &src->getPixels()[src->getBufferSize() - 1];
		//unsigned char * pdst = &dst->getPixels()[0];
		unsigned char * pmask = &mask->getPixels()[0];
		unsigned char * c = color.toColor32();

		while (ptr < end)
		{
			if (pmask[0] != 0 || pmask[1] != 0 || pmask[2] != 0)
			{
				//float a = sqrtf((float)pmask[0] * (float)pmask[0] + (float)pmask[1] * (float)pmask[1] + (float)pmask[2] * (float)pmask[2]) / 255.f;
				float a = ((pmask[0] + pmask[1] + pmask[2]) / 3.f) / 255.f;

				*ptr++ = ((float)c[0] * a) + ((float)*ptr * (1 - a));
				pmask++;

				*ptr++ = ((float)c[1] * a) + ((float)*ptr * (1 - a));
				pmask++;

				*ptr++ = ((float)c[2] * a) + ((float)*ptr * (1 - a));
				pmask++;

				ptr++;
				pmask++;
			}
			else
			{
				ptr += 4;
				pmask += 4;
			}
		}
	}
}