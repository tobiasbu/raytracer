
#include "image/filter.h"
#include <sstream>

Filter::Filter(const int & size)
{
	int nsize = size;

	if (size <= 2)
		nsize = 3;

	// avoid non odd numbers
	if (nsize % 2 == 0)
		nsize++;

	this->_height = size;
	this->_width = size;
	this->_size = _width * _height;
	this->kernel = new float[this->_size];
}

Filter::~Filter()
{
	if (kernel || kernel != NULL || kernel != nullptr)
		delete kernel;

	kernel = nullptr;
}

void Filter::buildIdentity(Filter * k, const float & middle, const float & fill)
{
	int imiddle = (k->_width / 2) + ((k->_height / 2) * k->_width);

	if (imiddle <= 0)
		imiddle = 1;

	for (int i = 0; i < k->_size; i++)
		k->kernel[i] = fill;

	k->kernel[imiddle] = middle;
}

void Filter::buildIdentityCircular(Filter * k, const float & middle, const float & fill, const float & outfill)
{
	int middleW = k->_width / 2;
	int middleH = k->_height / 2;
	int halfMiddle = middleW / 2;
	int auxW = 0;

	if (halfMiddle <= 0)
	{
		halfMiddle = middleW;
		//auxW -= halfMiddle;
	}

	if (k->_width >= 9)
	{
		halfMiddle = 1;
	}
	//&& (kx < minmiddleW || kx > maxmiddleW)

	for (int i = 0; i < k->_size; i++)
	{
		int kx = i % k->_width;
		int ky = i / k->_height;

		if (kx < middleW - auxW || kx > middleW + auxW)
			k->kernel[i] = outfill;
		else
			k->kernel[i] = fill;

		if (kx >= k->_width - 1) // new y
		{
			if (ky < middleH)
				auxW += halfMiddle;
			else
				auxW -= halfMiddle;
		}

	}

	if (middle != fill)
		k->kernel[middleW + middleH * k->_width] = middle;
}

std::string Filter::toString()
{
	if (_width <= 0 || _height <= 0 || !kernel)
	{
		return "Filter( Invalid )";
	}
	else
	{
		std::stringstream s;

		s << "Filter( \n size:" << this->_width << " div: " << (1.0f / divisor) << " off: " << offset << "\n";

		for (int i = 0; i < _size; i++)
		{
			s << kernel[i] << "\t";

			if ((i+1) % _height == 0)
				s << '\n';

		}

		s << " )";

		return s.str();
	}
}

void Filter::apply(Image * src, Image * dst)
{
	// check validity of params
	if (!src || !kernel)
		return;

	if (src->getWidth() <= 0 || src->getHeight() <= 0)
		return;

	if (_width <= 0 || _height <= 0)
		return;

	unsigned int width = src->getWidth();
	unsigned int height = src->getHeight();
	// accumulator for each channel (no alpha channel)
	float sum[3];
	int kCenterX, kCenterY;
	// to check boundary of input array
	//int rowMin, rowMax;
	//int colMin, colMax;


	//float *kPtr;
	//unsigned char * pixels = src->getPixels();
	//unsigned char *inPtr, *inPtr2, *outPtr;

	Image * s = nullptr;

	if (src == dst)
		s = src->clone();
	else
		s = src;

	// find center position of kernel (half of kernel size)
	kCenterX = _width >> 1;
	kCenterY = _height >> 1;
	int p = 0;


	// init working  pointers
	const unsigned char * pixels = s->getPixelsPtr();
	unsigned char * out = &dst->getPixels()[0];
	

	for (int i = 0; i < height; ++i)                // rows
	{
		for (int j = 0; j < width; ++j)            // columns
		{

			if ((j == 0) || (j == width - 1) || (i == 0) || (i == height - 1))
			{

				out += 4;
				continue;
			}
			else 
			{

				// set to 0 before accumulate
				sum[0] = 0; sum[1] = 0; sum[2] = 0;

				for (int kx = -kCenterX; kx <= kCenterX; kx++)
				{
					int px = j + kx;

					if (px >= 0 && px < width /*&& py >= 0 && py < height*/) 
					{

						for (int ky = -kCenterY; ky <= kCenterY; ky++)
						{
							int py = i + ky;

						if (py >= 0 && py < height) 
						{

						
								/*sum[0] += *(kPtr+n+2) * *inPtr; //[j + 2][i + 2]
								sum[1] += *(kPtr+n+2) * *inPtr++;
								sum[2] += *(kPtr+n+2) * *inPtr++;*/

								int k = (kx + kCenterX) + (ky + kCenterY) * _width;
							
							

								//if ((px < 0) || (px >= dataSizeX) || (py < 0) || (py >= dataSizeY))
							

									p = (px + py * width) * 4;

									float kern = kernel[k] / divisor;

									sum[0] += kern * (float)pixels[p] + offset;
									sum[1] += kern * (float)pixels[p + 1] + offset;
									sum[2] += kern * (float)pixels[p + 2] + offset;
						}

								/*sum[0] += sum_r + kernel[j + 2][i + 2] * r;
								sum[1] += sum_g + kernel[j + 2][i + 2] * g;
								sum[2] += sum_b + kernel[j + 2][i + 2] * b;*/


								//++kPtr;
						}
					}
					else
						continue;
					//kPtr += _kernel_width;
				}

				/*sum[0] = sum[0] / divisor + offset;
				sum[1] = sum[1] / divisor + offset;
				sum[2] = sum[2] / divisor + offset;*/



				*out++ = (unsigned char)(Math::clamp(sum[0], 0.0, 255.f));
				*out++ = (unsigned char)(Math::clamp(sum[1], 0.0, 255.f));
				*out++ = (unsigned char)(Math::clamp(sum[2], 0.0, 255.f));

				/**out++ =sum[0];
				*out++ = sum[1];
				*out++ = sum[2];*/

				out++;
			}
			/**outPtr++ = (unsigned char)sum;
			*outPtr++ = (unsigned char)sum;
			*outPtr++ = (unsigned char)sum;
			outPtr++;*/
			/**outPtr++ = (unsigned char)((float)fabs(sum[0]) + 0.5f);
			*outPtr++ = (unsigned char)((float)fabs(sum[1]) + 0.5f);
			*outPtr++ = (unsigned char)((float)fabs(sum[2]) + 0.5f);

			// increment pointers
			// skip alpha
			outPtr++;*/
			//inPtr2 += 4; // skip for channels
			//inPtr = inPtr2;

		}
	}

	if (src == dst)
		delete s;


}

bool Filter::convolve(const Image * src, const Image *& out, float * kernel, int kernelSizeX, int kernelSizeY, float divisor, float offset) {

	// check validity of params
	if (!src || !kernel || !out)
		return false;

	if (src->getWidth() <= 0 || src->getHeight() <= 0)
		return false;

	if (kernelSizeX <= 0 || kernelSizeY <= 0)
		return false;

	unsigned int dataSizeX = src->getWidth();
	unsigned int dataSizeY = src->getHeight();
	// accumulator for each channel (no alpha channel)
	float sum[3];
	int kCenterX, kCenterY;

	float *kPtr;
	unsigned char * pixels = src->getPixels();
	//unsigned char *inPtr, *inPtr2, *outPtr;

	//out = new Image(dataSizeX, dataSizeY);

	// find center position of kernel (half of kernel size)
	kCenterX = kernelSizeX / 2;
	kCenterY = kernelSizeY / 2;
	int p = 0;
	int px = 0;
	int py = 0;
	int k = 0;

	unsigned char * outpixels = &out->getPixels()[0];
	unsigned char * outPtr = outpixels;

	// for channels, so update data size
	//float divisor = 1;
	//float offset = 0;

	for (int i = 0; i < dataSizeY; ++i)                // rows
	{

		for (int j = 0; j < dataSizeX; ++j)            // columns
		{

			// set to 0 before accumulate
			sum[0] = 0; sum[1] = 0; sum[2] = 0;

			for (int kx = -kCenterX; kx <= kCenterX; kx++)
			{
				// kernel rows
				for (int ky = -kCenterY; ky <= kCenterY; ky++)
				{


					/*sum[0] += *(kPtr+n+2) * *inPtr; //[j + 2][i + 2]
					sum[1] += *(kPtr+n+2) * *inPtr++;
					sum[2] += *(kPtr+n+2) * *inPtr++;*/

					k = (kx + kCenterX) + (ky + kCenterY) * kernelSizeX;
					px = j + kx;
					py = i + ky;

					//if ((px < 0) || (px >= dataSizeX) || (py < 0) || (py >= dataSizeY))
					if (px >= 0 && px < dataSizeX && py >= 0 && py < dataSizeY) {

						p = (px + py * dataSizeX) * 4;

						float kern = kernel[k];

						sum[0] += kern * (float)pixels[p];
						sum[1] += kern * (float)pixels[p + 1];
						sum[2] += kern * (float)pixels[p + 2];
					}
				}
			}

			sum[0] /= divisor + offset;
			sum[1] /= divisor + offset;
			sum[2] /= divisor + offset;

			*outPtr++ = (unsigned char)(Math::clamp(sum[0], 0.0, 255.f));
			*outPtr++ = (unsigned char)(Math::clamp(sum[1], 0.0, 255.f));
			*outPtr++ = (unsigned char)(Math::clamp(sum[2], 0.0, 255.f));


			outPtr++;
			/**outPtr++ = (unsigned char)((float)fabs(sum[0]) + 0.5f);
			*outPtr++ = (unsigned char)((float)fabs(sum[0]) + 0.5f);
			*outPtr++ = (unsigned char)((float)fabs(sum[1]) + 0.5f);
			outPtr*/

		}
	}


}

Sharpen::Sharpen(const int & size)
	:
	Filter(size)
{
	//(float)ceilf(((float)_size / 2.0f))
	buildIdentityCircular(this, (_size+1) / 2, -1); // (float)ceilf((float)(size * 0.55555555555555555555555555555556f))
	
}

Sharpen9::Sharpen9(const int & size)
	:
	Filter(size)
{
	buildIdentity(this, (float)_size, -1);
}


BoxBlur::BoxBlur(const int & size)
	:
	Filter(size)
{
	for (int i = 0; i < _size; i++)
		kernel[i] = 1.f;

	divisor = _size; // 1.0f / ceilf((float)(k->_size) / 2.0f);
}

SmoothBlur::SmoothBlur(const int & size)
:
Filter(size)
{
	buildIdentityCircular(this, 1, 1, 0);

	divisor = (_size + 1) / 2;//floorf((float)(_size) / 2.0f);//1.0f / ceilf((float)(k->_size) / 2.0f);


}

MotionBlur::MotionBlur(const int & size)
	:
	Filter(size)
{
	for (int i = 0; i < _size; i++)
	{
		int kx = i % _width;
		int ky = i / _height;

		if (kx == ky || (_width - 1) - kx == ky)
			kernel[i] = 1.f;
		else
			kernel[i] = 0;
	}

	divisor = (_width + _height) - 1; //1.0f / ((_width + _height) - 1);
}

Emboss::Emboss(const int & size)
	:
	Filter(3) // i have only found 3x3 matrix for Emboss
{
	kernel[0] = -2; kernel[1] = -1; kernel[2] = 0;
	kernel[3] = -1; kernel[4] = 1; kernel[5] = 1;
	kernel[6] = 0; kernel[7] = 1; kernel[8] = 2;
}

Laplacian::Laplacian(const int & size)
	:
	Filter(3) // laplacian too
{


	kernel[0] = -1; kernel[1] = -1; kernel[2] = -1;
	kernel[3] = -1; kernel[4] = 8; kernel[5] = -1;
	kernel[6] = -1; kernel[7] = -1; kernel[8] = -1;


	divisor = 1;

}

UnsharpMask::UnsharpMask(const int & size)
	:
	Filter(5) // and so on
{


	kernel[0] = 1; kernel[1] = 4; kernel[2] = 6;  kernel[3] = 4;  kernel[4] = 1;
	kernel[5] = 4; kernel[6] = 16; kernel[7] = 24;  kernel[8] = 16;  kernel[9] = 4;
	kernel[10] = 6; kernel[11] = 24; kernel[12] = -476; kernel[13] = 24; kernel[14] = 6;
	kernel[15] = 4; kernel[16] = 16; kernel[17] = 24;  kernel[18] = 16;  kernel[19] = 4;
	kernel[20] = 1; kernel[21] = 4; kernel[22] = 6;  kernel[23] = 4;  kernel[24] = 1;

	divisor = -256.f;

}

DifferenceOperator::DifferenceOperator(const int & size)
	:
	Filter(size)
{
	kernely = new float[_size];
}

DifferenceOperator::~DifferenceOperator()
{
	if (kernely || kernely != NULL || kernely != nullptr)
		delete kernely;

	kernely = nullptr;
}

std::string DifferenceOperator::toString()
{
	std::stringstream s;

	if (type == FilterType::Sobel)
		s << "Sobel";
	else if (type == FilterType::Scharr)
		s << "Scharr";
	else
		s << "DifferenceOperator";

	if (_width <= 0 || _height <= 0 || !kernel || !kernely)
		s << "(Invalid)";
	else
	{

		s << "( \ndiv: " << (1.0f / divisor) << " off: " << offset << "\n";

		s << "Gx: (\n";

		int i;

		for (i = 0; i < _size; i++)
		{
			s << kernel[i] << " ";

			if ((i + 1) % _height == 0)
				s << '\n';

		}

		s << ")\nGy: (\n";

		for (i = 0; i < _size; i++)
		{
			s << kernely[i] << " ";

			if ((i + 1) % _height == 0)
				s << '\n';

		}

		s << ")\n)";

		return s.str();
	}

	return s.str();
}

void DifferenceOperator::apply(Image * src, Image * dst)
{
	apply(src, dst, 1);
}

void DifferenceOperator::apply(Image * src, Image * dst, const int & channels, bool dotted)
{
	if (!src || !dst)
		return;

	if (src->getWidth() <= 0 || src->getHeight() <= 0)
		return;

	

	/*float gx[] = {
		-1, 0, 1,
		-2, 0, 2,
		-1, 0, 1
	};

	float gy[] = {
		1, 2, 1,
		0, 0, 0,
		-1, -2, -1
	};*/

	Image * s;

	if (src == dst)
		s = src->clone();
	else
		s = src;

	const int c = (channels <= 0) ? 1 : (channels > 3) ? 3 : channels;

	

	unsigned int width = src->getWidth();
	unsigned int height = src->getHeight();
	int kCenterX, kCenterY;
	
	// find center position of kernel (half of kernel size)
	kCenterX = _width >> 1;
	kCenterY = _height >> 1;

	// accumulator for each channel (no alpha channel)
	float valuex[3]; 
	float valuey[3];


	int p = 0;
	const unsigned char * pixels = s->getPixelsPtr();
	unsigned char * dptr = &dst->getPixels()[0];

	

	for (int i = 0; i < height; ++i)                // rows
	{
		for (int j = 0; j < width; ++j)            // columns
		{

			if ((j == 0) || (j == width - 1) || (i == 0) || (i == height - 1))
			{
				dptr += 4;
				continue;
			}
			else
			{
				valuex[0] = 0;
				valuey[0] = 0;

				if (c > 1) {
					valuex[1] = 0, valuex[2] = 0;
					valuey[1] = 0, valuey[2] = 0;
				}
				

				// kernel rows
				for (int ky = -kCenterY; ky <= kCenterY; ky++)
				{
					int py = i + ky;

					if (py >= 0 && py < height) {

						for (int kx = -kCenterX; kx <= kCenterX; kx++)
						{
							int k = (kx + kCenterX) + (ky + kCenterY) * 3;
							int px = j + kx;
							p = (px + py * width) * 4;

							valuex[0] += kernel[k] * (float)pixels[p];
							valuey[0] += kernely[k] * (float)pixels[p];

							if (c > 1) {
								valuex[1] += kernel[k] * (float)pixels[p + 1];
								valuey[1] += kernely[k] * (float)pixels[p + 1];
								valuex[2] += kernel[k] * (float)pixels[p + 2];
								valuey[2] += kernely[k] * (float)pixels[p + 2];

							}
							
						}

					}
					else
						continue;

				}

				//Math::clamp(abs(valuex[0]) + abs(valuey[0]), 0, 255);
				float valuer;
				if (!dotted)
					valuer = Math::clamp(sqrtf(valuex[0] * valuex[0] + valuey[0] * valuey[0]), 0.f, 255.f);
				else
					valuer = Math::clamp(abs(valuex[0]) + abs(valuey[0]), 0, 255);
				//valuer = (valuer * 2.f > 255) ? 255 : roundf(valuer * 2.f);

				if (c > 1) {
					float valueg, valueb;
					if (!dotted) {
						valueg  = Math::clamp(sqrtf(valuex[1] * valuex[1] + valuey[1] * valuey[1]), 0.f, 255.f);
						valueb = Math::clamp(sqrtf(valuex[2] * valuex[2] + valuey[2] * valuey[2]), 0.f, 255.f);
					}
					else
					{
						valueg = Math::clamp(abs(valuex[1]) + abs(valuey[1]), 0.f, 255.f);
						valueb = Math::clamp(abs(valuex[2]) + abs(valuey[2]), 0.f, 255.f);
					}

					*dptr++ = (unsigned char)valuer;
					*dptr++ = (unsigned char)valueg;
					*dptr++ = (unsigned char)valueb;
				}
				else {
					
					*dptr++ = valuer;
					*dptr++ = valuer;
					*dptr++ = valuer;
				}



				dptr++;
			}
		}
	}

	if (src == dst)
		delete s;
}

Scharr::Scharr(const int & size)
	:
	DifferenceOperator(size)
{

	type = FilterType::Scharr;

	float * kx = kernel;
	float * ky = kernely;

	if (size == 3)
	{
		kx[0] = -3, kx[1] = 0, kx[2] = 3;
		kx[3] = -10, kx[4] = 0, kx[5] = 10;
		kx[6] = -3, kx[7] = 0, kx[8] = 3;

		ky[0] = -3, ky[1] = -10, ky[2] = -3;
		ky[3] = 0, ky[4] = 0, ky[5] = 0;
		ky[6] = 3, ky[7] = 10, ky[8] = 3;
	}
}

Sobel::Sobel(const int & size)
	:
	DifferenceOperator(size)
{
	type = FilterType::Sobel;

	int order = 1;
	float * kx = kernel;
	float * ky = kernely;
	int i, j;

	if (size == 3)
	{
		kx[0] = 1, kx[1] = 0, kx[2] = -1;
		kx[3] = 2, kx[4] = 0, kx[5] = -2;
		kx[6] = 1, kx[7] = 0, kx[8] = -1;

		ky[0] = 1, ky[1] = 2, ky[2] = 1;
		ky[3] = 0, ky[4] = 0, ky[5] = 0;
		ky[6] = -1, ky[7] = -2, ky[8] = -1;
	}

	// TODO

	/*else
	{

	int side = _width * 2;// +3;
	int halfSide = side / 2;
	int i, j;
	for (j = 0; j < _height; j++)
	{
	for (i = 0; i < _width; i++)
	{
	int idx = i + j * _width;

	if (idx >= side && idx < side + halfSide)
	ky[idx] = 0;

	/*int k = (i <= halfSide) ? (halfSide + i) : (side + halfSide - i - 1);
	for (int j = 0; j < side; j++)
	{

	int ix = i + j * side;
	int iy = j + i * side;
	float t = 0;

	if (j < halfSide)
	t = j - k; // ky[iy] =
	else if (j > halfSide)
	t = k - (side - j - 1);
	else
	t = 0;

	std::cout << t << " ";
	//kx[ix] = t;
	}
	}

	//std::cout << idx << std::endl;
	}*/




}

EdgeExtractor::EdgeExtractor(const int & size)
	:
	DifferenceOperator(size)
{
	if (size == 3)
	{
		kernel[0] = 0; kernel[1] = 0; kernel[2] = 0;
		kernel[3] = 2; kernel[4] = -4; kernel[5] = 2;
		kernel[6] = 0; kernel[7] = 0; kernel[8] = 0;

		kernely[0] = 0; kernely[1] = 2; kernely[2] = 0;
		kernely[3] = 0; kernely[4] = -4; kernely[5] = 0;
		kernely[6] = 0; kernely[7] = 2; kernely[8] = 0;
	}
	else
	{
		/*kernel[0] = 0; kernel[1] = 1; kernel[2] = 2;  kernel[3] = 1;  kernel[4] = 1;
		kernel[5] = 1; kernel[6] = 8; kernel[7] = 16;  kernel[8] = 8;  kernel[9] = 4;
		kernel[10] = 2; kernel[11] = 16; kernel[12] = -116; kernel[13] = 16; kernel[14] = 2;
		kernel[15] = 1; kernel[16] = 8; kernel[17] = 16; kernel[18] = 8;  kernel[19] = 4;
		kernel[20] = 0; kernel[21] = 1; kernel[22] = 2;  kernel[23] = 1;  kernel[24] = 1;*/


	}

	divisor = 1;
	offset = 0;

}

/*

//// BACKUPS

/*
Filter * Filter::build(Filter::FilterType type, const int & size)
{

Filter * k = new Filter(size);

//switch (type) {

case Identity:
{
buildIdentity(k, 1, 0);
}

case BoxBlur: {
for (int i = 0; i < k->_size; i++)
k->kernel[i] = 1.f;

k->divisor = 1.0f / (float)k->_size;

break;
}

case SmoothBlur: {

buildIdentityCircular(k, 1, 1, 0);

k->divisor = 1.0f / ceilf((float)(k->_size) / 2.0f);

break;
}

case MotionBlur: // both sides 45 - 135 degrees
{
for (int i = 0; i < k->_size; i++)
{
int kx = i % k->_width;
int ky = i / k->_height;

if (kx == ky || (k->_width-1) - kx == ky)
k->kernel[i] = 1.f;
else
k->kernel[i] = 0;
}

k->divisor = 1.0f / ((k->_width + k->_height)-1);

break;
}

case Sharpen9:
{
buildIdentity(k, (float)size, -1);
break;
}

case Sharpen5:
{
buildIdentityCircular(k, (float)ceilf((float)(size * 0.55555555555555555555555555555556f)), -1);
break;
}

case SobelGy:
{

/*float gy[] = {
1, 2, 1,
0, 0, 0,
-1, -2, -1
};*/

/*int ksize = nsize;
int order = 1;
float * ker = k->kernel;
int i, j;

for (int k = 0; k < 2; k++)
{
//Mat* kernel = k == 0 ? &kx : &ky;
int order = k == 0 ? 0 : 1;

if (ksize == 1)
ker[0] = 1;
else if (ksize == 3)
{
if (order == 0)
ker[0] = 1, ker[1] = 2, ker[2] = 1;
else if (order == 1)
ker[0] = -1, ker[1] = 0, ker[2] = 1;
else
ker[0] = 1, ker[1] = -2, ker[2] = 1;
}
else
{
int oldval, newval;
ker[0] = 1;
for (i = 0; i < ksize; i++)
ker[i + 1] = 0;

for (i = 0; i < ksize - order - 1; i++)
{
oldval = ker[0];
for (j = 1; j <= ksize; j++)
{
newval = ker[j] + ker[j - 1];
ker[j - 1] = oldval;
oldval = newval;
}
}

for (i = 0; i < order; i++)
{
oldval = -ker[0];
for (j = 1; j <= ksize; j++)
{
newval = ker[j - 1] - ker[j];
ker[j - 1] = oldval;
oldval = newval;
}
}
}

}

break;
}

}

//return k;
}*/

/*
void Filter::sobelOperator(Image * src, Image * dst)
{
	// check validity of params
	if (!src || !dst)
		return;

	if (src->getWidth() <= 0 || src->getHeight() <= 0)
		return;

	float gx[] = {
		-1, 0, 1,
		-2, 0, 2,
		-1, 0, 1
	};

	float gy[] = {
		1, 2, 1,
		0, 0, 0,
		-1, -2, -1
	};

	unsigned int width = src->getWidth();
	unsigned int height = src->getHeight();
	// accumulator for each channel (no alpha channel)
	//float sum[3];
	//float sumX[3];
	//float sumY[3];
	float sumx = 0;
	float sumy = 0;

	
	int kCenterX, kCenterY;
	int p = 0;
	// to check boundary of input array
	//int rowMin, rowMax;
	//int colMin, colMax;

	float *kPtr;
	

	// find center position of kernel (half of kernel size)
	kCenterX = 3 >> 1;
	kCenterY = 3 >> 1;

	//if (out == nullptr || out == NULL || !out)
	//Image * out = src->clone();

	// init working  pointers
	//inPtr = inPtr2 = &pixels[0];  // dataSizeX * kCenterY + kCenterX note that  it is shifted (kCenterX, kCenterY),
	unsigned char * ptr = &src->getPixels()[0];
	unsigned char * pixels = src->getPixels();
	unsigned char * dptr = &dst->getPixels()[0];
	//unsigned char * outPtr = outpixels;

	unsigned int buffer = width * height * 4;
	unsigned int i = 0;
	float valuex = 0, valuey = 0;
	int x = 0, y = 0;
	int value = 0;

	for (int i = 0; i < height; ++i)                // rows
	{
		for (int j = 0; j < width; ++j)            // columns
		{
			

			if ((j == 0) || (j == width - 1) || (i == 0) || (i == height - 1))
			{

				dptr += 4;
				//outPtr++;
				//outPtr++;
				//outPtr++;
				continue;
			}
			else 
			{
				valuex = 0;
				valuey = 0;

				// kernel rows
				for (int ky = -kCenterY; ky <= kCenterY; ky++)
				{
					int py = i + ky;

					if (py >= 0 && py < height) {

						for (int kx = -kCenterX; kx <= kCenterX; kx++)
						{
							int k = (kx + kCenterX) + (ky + kCenterY) * 3;
							int px = j + kx;
							p = (px + py * width) * 4;

							valuex += gx[k] * (float)pixels[p];
							valuey += gy[k] * (float)pixels[p];
						}

					}
					else
						continue;

				}

				unsigned char value = (unsigned char)Math::clamp(ceilf(sqrtf((valuex*valuex) + (valuey*valuey))),0,255);
				

				*dptr++ = value;
				*dptr++ = value;
				*dptr++ = value;


				dptr++;
			}
		}
	}


}
*/

/*for (int i = 0; i < dataSizeY; ++i)                // rows
{


	// compute the range of convolution, the current row of kernel should be between these
	//rowMax = i + kCenterY;
	//rowMin = i - dataSizeY + kCenterY;

	for (int j = 0; j < dataSizeX; ++j)            // columns
	{


		// compute the range of convolution, the current column of kernel should be between these
		//colMax = j + kCenterX;
		//colMin = j - dataSizeX + kCenterX;

		// set to 0 before accumulate
		//sumX[0] = 0; sumX[1] = 0; sumX[2] = 0;
		//sumY[0] = 0; sumY[1] = 0; sumY[2] = 0;
		sumx = 0;
		sumy = 0;




		// kernel rows
		for (int ky = -kCenterY; ky <= kCenterY; ky++)
		{
			int py = i + ky;

			if (py >= 0 && py < dataSizeY) {

				for (int kx = -kCenterX; kx <= kCenterX; kx++)
				{
					//-1+1 = 0
					//-1+1 = 0
					int k = (kx + kCenterX) + (ky + kCenterY) * 3;
					int px = j + kx;

					if (px >= 0 && px < dataSizeX) {

						p = (px + py * dataSizeX) * 4;
						sumx += hy[k] * (float)pixels[p];
						sumy += hx[k] * (float)pixels[p];*/

						/*if (pixels[p] > pixels[p + 1] && pixels[p] > pixels[p + 2])
						{
						sumx += hx[k] * (float)pixels[p];
						sumy += hy[k] * (float)pixels[p];
						}
						else if (pixels[p+1] > pixels[p] && pixels[p+1] > pixels[p + 2])
						{
						sumx += hx[k] * (float)pixels[p+1];
						sumy += hy[k] * (float)pixels[p+1];
						}
						else
						{

						sumx += hx[k] * (float)pixels[p + 2];
						sumy += hy[k] * (float)pixels[p + 2];
						//}

						/*sumX[0] += hx[k] * (float)pixels[p];
						sumX[1] += hx[k] * (float)pixels[p + 1];
						sumX[2] += hx[k] * (float)pixels[p + 2];

						sumY[0] += hy[k] * (float)pixels[p];
						sumY[1] += hy[k] * (float)pixels[p + 1];
						//sumY[2] += hy[k] * (float)pixels[p + 2];*/
/*					}

				}
			}
			else
				continue;
		}*/


		/*sum[0] = abs(sumX[0]) + abs(sumY[0]);
		sum[1] = abs(sumX[1]) + abs(sumY[1]);
		sum[2] = abs(sumX[2]) + abs(sumY[2]);*/
		//abs(sumx) + abs(sumy);
		// sqrt((sumx*sumx) + (sumy*sumy))
		/*unsigned char sum = (unsigned char)Math::clamp(abs(sumx) + abs(sumy), 0.0, 255.f);


		*outPtr++ = sum;
		*outPtr++ = sum;
		*outPtr++ = sum;


		outPtr++;


	}
}*/