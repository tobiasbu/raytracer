
#include "image/imagecolors.h"
#include "image/filter.h"

// transform image colors to grayscale
bool ImageColors::grayscale(Image * image, const GrayscaleType & method)
{
	if (!image)
		return false;

	if (image->getWidth() <= 0 || image->getHeight() <= 0)
		return false;

	unsigned char * ptr = &image->getPixels()[0];
	unsigned char * end = &image->getPixels()[image->getBufferSize() - 1];
	float val = 0;

	while (ptr < end)
	{
		switch (method) {
		default:
		case GrayscaleType::Average: val = (((float)ptr[0] + (float)ptr[1] + (float)ptr[2]) / 3.f); break;
		case GrayscaleType::Desaturation: val = (Math::max((float)ptr[0], Math::max((float)ptr[1], (float)ptr[2])) + Math::min((float)ptr[0], Math::min((float)ptr[1], (float)ptr[2]))) / 2; break;
		case GrayscaleType::Luminance: val = (float)ptr[0] * 0.299f + (float)ptr[1] * 0.587f + (float)ptr[2] * 0.114f; break; // CCIR 601 - BT 601
		case GrayscaleType::Luma: val = (float)ptr[0] * 0.2126f + (float)ptr[1] * 0.7152f + (float)ptr[2] * 0.0722f; break; // BT 709 - relative luminance
		case GrayscaleType::HSP: 
		{ 
			val = sqrtf((float)ptr[0] * (float)ptr[0] * 0.299f + (float)ptr[1] * (float)ptr[1] * 0.587f + (float)ptr[2] * (float)ptr[2] * 0.0722f); 
			break;
		}
		case GrayscaleType::BlueChannel: val = (float)ptr[2]; break;
		case GrayscaleType::MaxDecomposition: val = (Math::max(ptr[0], Math::max(ptr[1], ptr[2]))); break;
		case GrayscaleType::MinDecomposition: val = (Math::min(ptr[0], Math::min(ptr[1], ptr[2]))); break;
		}



		*ptr++ = (unsigned char)val;
		*ptr++ = (unsigned char)val;
		*ptr++ = (unsigned char)val;
		ptr++;
		
	}

	/*for (size_t y = 0; y < image->getHeight(); ++y)
	{
		for (size_t x = 0; x < image->getWidth(); ++x)
		{
			Color col = image->getPixelColor(x, y);

			// quick and dirty grayscale method
			switch (method) {
			default:
			case GrayscaleType::Average:	col.set((col.r + col.g + col.b) / 3.f); break;
			case GrayscaleType::Luminance: col.set(col.r * 0.3f + col.g * 0.59f + col.b * 0.11f); break;
			case GrayscaleType::Luma: col.set(col.r * 0.2126f + col.g * 0.7152f + col.b * 0.0722f); break;
			case GrayscaleType::LumaBT601: col.set(col.r * 0.299f + col.g * 0.587f + col.b * 0.114f); break;
			case GrayscaleType::Desaturation: col.set((Math::max(col.r, Math::max(col.g, col.b)) + Math::min(col.r, Math::min(col.g, col.b))) / 2); break;
			case GrayscaleType::MaxDecomposition: col.set(Math::max(col.r, Math::max(col.g, col.b))); break;
			case GrayscaleType::MinDecomposition: col.set(Math::min(col.r, Math::min(col.g, col.b))); break;
			case GrayscaleType::RedChannel: col.set(col.r); break;
			case GrayscaleType::BlueChannel: col.set(col.g); break;
			case GrayscaleType::GreenChannel: col.set(col.b); break;
			}

			image->setPixel(x, y, col);



		}
	}*/

	return true;
}

// posterize image
// level beteween 2 and 256
bool ImageColors::posterize(Image * image, int level) {

	// check http://www.axiomx.com/posterize.htm

	if (!image)
		return false;

	if (image->getWidth() <= 0 || image->getHeight() <= 0)
		return false;

	level = Math::clampint(level, 2, 256);

	// conversion factor
	//float factor = 255.f / (float)level;
	float value = 255.f / (float)(level - 1);

	for (size_t y = 0; y < image->getHeight(); ++y)
	{
		for (size_t x = 0; x < image->getWidth(); ++x)
		{
			unsigned char * col = image->getPixel(x, y);

			for (size_t k = 0; k < 3; ++k) {

				// more like gimp
				col[k] = (int)(col[k] / value + 0.5f) * value;

				// ALGORITHM 2
				/* is too dark
				float cfloat = col[k] / factor;

				// integer color (round)
				int cint = (int)(cfloat);

				// If the number was rounded to the higher value, subtract 1 from it.
				if (cint > cfloat)
				cint = cint - 1;

				// Multiply the number of color values by pixel's color area to get the fraction.
				float newcfloat = value * cint;

				// and round it.
				int newcint = (int)(newcfloat);

				// If the number was rounded to the higher value, subtract 1 from it.
				if (newcint > newcfloat)
				newcint = newcint - 1;

				// set color component
				col[k] = newcint;*/
			}

			image->setPixel(x, y, col);
		}

	}

	return true;

}


bool ImageColors::contrastBrightnes(Image * image, float c, float b) {

	if (!image)
		return false;

	if (image->getWidth() <= 0 || image->getHeight() <= 0)
		return false;

	if (b < -255)
		b = -255;
	else if (b > 255)
		b = 255;	

	/*if (c > 3.0)
		c = 3.0f;
	else if (c < 1.0)
		c = 1.0;*/

	float fc = (259 * (c + 255)) / (255 * (259 - c));

	for (size_t y = 0; y < image->getHeight(); ++y)
	{
		for (size_t x = 0; x < image->getWidth(); ++x)
		{
			unsigned char * a = image->getPixel(x, y);

			for (int i = 0; i < 3; i++) {
				//a[i] = (unsigned char)Math::clampint((c * ((float)a[i] - 128) + 128.f + b), 0, 256);
				a[i] = (unsigned char)Math::clamp((((fc * ((float)a[i] - 128.f)) + 128.f) + b) , 0.f, 255.f);
			}

			image->setPixel(x, y, a);

			// quick and dirty grayscale method
			//switch (method) {
			//default:
			//case GrayscaleMethod::Average:	col.set((col.r + col.g + col.b) / 3.f); break;
			//}
		}
	}

}

bool ImageColors::threshold(Image * image, int level)
{
	// check http://www.axiomx.com/posterize.htm

	if (!image)
		return false;

	if (image->getWidth() <= 0 || image->getHeight() <= 0)
		return false;

	unsigned char * ptr = &image->getPixels()[0];
	unsigned char * end = &image->getPixels()[image->getBufferSize() - 1];
	unsigned char val = 0;

	while (ptr < end)
	{
		// binary >
		// binary inv <=
		// trunc std::min
		// to zero >
		// to zero inv <=

		if (*ptr < level)
			val = 0;
		else
			val = 255;

		*ptr++ = val;
		//*ptr++ = (unsigned char)255 - *ptr;
		//*ptr++ = (unsigned char)255 - *ptr;
		*ptr++ = val;
		*ptr++ = val;
		ptr++;
	}

	return true;
}

bool ImageColors::adaptiveThreshold(Image * src, Image * dst, int blockSize, int level, int delta )
{
	if (!src)
		return false;

	if (src->getWidth() <= 0 || src->getHeight() <= 0)
		return false;

	if (blockSize <= 1)
		blockSize = 3;

	if (blockSize % 2 == 1)
		blockSize++;

	Image * d;
	Image * mean = src->clone();

	if (src == dst)
		d = src->clone();
	else
		d = dst;

	int i, j;
	long sum = 0;
	int count = 0;
	int index;
	int x1, y1, x2, y2;
	int s2 = blockSize / 2;

	const unsigned char * pixels = src->getPixelsPtr();
	unsigned char * dptr = &d->getPixels()[0];
	const unsigned char * end = &pixels[src->getBufferSize() - 1];
	unsigned char tab[768];


	GaussianBlur g(blockSize);
	g.apply(src, mean);
	
	for (i = 0; i < 768; i++)
		tab[i] = (i - 255 > -delta ? level : 0);

	/*for (i = 0; i < src->getHeight(); i++)
	{
		for (j = 0; j < src->getWidth(); j++)
		{
			dptr[j]
		}

	}*/
	
	/*for (i = 0; i< src->getWidth(); i++)
	{
		// reset this column sum
		sum = 0;

		for (j = 0; j< src->getHeight(); j++)
		{
			index = j*src->getWidth() + i;

			sum += pixels[index];
			if (i == 0)
				integralImg[index] = sum;
			else
				integralImg[index] = integralImg[index - 1] + sum;
		}
	}*/
}

bool ImageColors::invert(Image * image)
{
	if (!image)
		return false;

	if (image->getWidth() <= 0 || image->getHeight() <= 0)
		return false;

	unsigned char * ptr = &image->getPixels()[0];
	unsigned char * end = &image->getPixels()[image->getBufferSize() - 1];

	while (ptr < end)
	{
		*ptr++ = *ptr ^ 0xffffff;//(unsigned char)255 - *ptr;
		*ptr++ = *ptr ^ 0xffffff;//(unsigned char)255 - *ptr;
		*ptr++ = *ptr ^ 0xffffff;//(unsigned char)255 - *ptr;
		ptr++;
	}

	return true;
}

bool ImageColors::colorize(Image * image, const float & h, const float & s, const float & v)
{
	unsigned char * ptr = &image->getPixels()[0];
	unsigned char * end = &image->getPixels()[image->getBufferSize() - 1];

	while (ptr < end)
	{
		*ptr++ = *ptr ^ 0xffffff;//(unsigned char)255 - *ptr;
		*ptr++ = *ptr ^ 0xffffff;//(unsigned char)255 - *ptr;
		*ptr++ = *ptr ^ 0xffffff;//(unsigned char)255 - *ptr;
		ptr++;
	}

	return true;
}

void ImageColors::RGBtoHSV(unsigned char * in, float & h, float & s, float & v)
{
	float delta, min;
	h = 0;

	min = Math::min(Math::min(in[0], in[1]), in[2]);
	v = Math::max(Math::max(in[0], in[1]), in[2]);
	delta = v - min;

	if (v == 0.0f)
		s = 0;
	else
		s = delta / v;

	if (s == 0)
		h = 0.0f;

	else
	{
		if ((float)in[0] == v)
			h = ((float)in[1] - (float)in[2]) / delta;
		else if ((float)in[1] == v)
			h = 2 + ((float)in[0] - (float)in[2]) / delta;
		else if ((float)in[2] == v)
			h = 4 + ((float)in[0] - (float)in[2]) / delta;

		h *= 60.f;

		if (h < 0.0f)
			h = h + 360.f;

		h /= 360.f;
	}
}

void ImageColors::antialiasing(Image * image, int samples)
{
	//unsigned char * ptr = &image->getPixels()[0];
	//unsigned char * end = &image->getPixels()[image->getBufferSize() - 1];

	float n2 = powf((float)samples, 2.f);
	int halfsample = samples / 2;
	Color f; // = { 0, 0, 0 };
	Color g;

	Image * s = image->clone();

	for (size_t y = 0; y < image->getHeight(); ++y)
	{
		for (size_t x = 0; x < image->getWidth(); ++x)
		{
			

			//sum[0] = 0; sum[1] = 0; sum[2] = 0;
			f = Color::clear;
			g = s->getPixelColor(x, y);
			//f = g;

			for (size_t sy = -halfsample; sy <= halfsample; ++sy)
			{
				int py = sy + y;

				if (py >= 0 && py < image->getHeight()) 
				{

					for (size_t sx = -halfsample; sx <= halfsample; ++sx)
					{
						int px = sx + x;

						
						if (px >= 0 && px < image->getWidth()) 
						{
							Color a = s->getPixelColor(px, py);
							f += a / n2;
							
							//f = a;
							
							
						}
						else
							continue;

					}
				}
				else
					continue;
			}

			 
			f.r = Math::clamp(f.r, 0, 1);
			f.g = Math::clamp(f.g, 0, 1);
			f.b = Math::clamp(f.b, 0, 1);
			f.a = g.a;
			//f = f / n2;
			//sum[1] = sum[1] / n2;
			//sum[2] = sum[2] / n2;



			image->setPixel(x, y, f);
		}

	}

	delete s;
	
}

void ImageColors::blend(Image * src, Image * dst, bool inversedBlend)
{
	if(!src || !dst)
		return;

	if (src->getWidth() <= 0 || src->getHeight() <= 0)
		return;

	unsigned char * ptr = &src->getPixels()[0];
	unsigned char * end = &src->getPixels()[src->getBufferSize() - 1];
	unsigned char * dptr = &dst->getPixels()[0];

	while (ptr < end)
	{
		//float a = ((dptr[0] + dptr[1] + dptr[2]) / 3.f) / 255.f;
		float a = sqrtf((float)dptr[0] * (float)dptr[0] + (float)dptr[1] * (float)dptr[1] + (float)dptr[2] * (float)dptr[2]) / 255.f;
		
		if (!inversedBlend)
			a = 1-a;

		*ptr++ = (*dptr * a) + (*ptr * (1 - a));
		dptr++;

		*ptr++ = (*dptr * a) + (*ptr * (1 - a));
		dptr++;

		*ptr++ = (*dptr * a) + (*ptr * (1 - a));
		dptr++;

		ptr++;
		dptr++;

		/**ptr++ = Math::min(*ptr + *dptr, *dptr);
		dptr++;*/

		/**ptr++ = (*ptr + *dptr++) / 2;
		*ptr++ = (*ptr + *dptr++) / 2;
		*ptr++ = (*ptr + *dptr++) / 2;
		ptr++;
		dptr++;*/
	}
}