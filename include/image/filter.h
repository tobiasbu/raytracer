
#ifndef _FILTER_H_
#define _FILTER_H_

#include "image.h"
#include "math/maths.h"

#include <assert.h>
#include <iostream>

class Filter
{


public:

	
	enum FilterType
	{
		Identity,
		BoxBlur, // median, low pass
		SmoothBlur,
		MotionBlur,
		Sharpen, // high pass
		Sharpen9, // high pass
		//SharpenLow, // sharpen and smooth
		Laplacian, // edege detection
		LaplacianDiagonals,// edege detection including diagonals
		Sobel,
		Scharr,
		Emboss,
		UnsharpMask,
		GaussianBlur

	};

protected:

	static void buildIdentityCircular(Filter * k, const float & middle = -1, const float & fill = -1, const float & outfill = 0);
	static void buildIdentity(Filter * k, const float & middle, const float & fill = -1);

	int _height;
	int _width;
	unsigned int _size = 0;
	float * kernel = nullptr;
	
	FilterType type;

public:

	float offset = 0;
	float divisor = 1;

	Filter(const int & size = 3);

	virtual ~Filter();

	// static functions

	// build filter unsued
	//static Filter * build(FilterType type, const int & size = 3);

	// convolve generic filter
	static bool convolve(const Image * src, const Image *& dst, float * kernel, int kernelSizeX, int kernelSizeY, float divisor = 1, float offset = 0);
	
	// functions

	// apply filter to image
	virtual void apply(Image * src, Image * dst);
	// rebuild filter
	//virtual void rebuild(const int & size = 3) {}
	virtual std::string toString();

	friend std::ostream& operator << (std::ostream &s, const Filter &k)
	{
		if (k._width <= 0 || k._height <= 0)
		{
			return s << "KernelConv( Invalid )";
		}
		else 
		{
			s << "KernelConv( ";

			for (int i = 0; i < k._size; i++)
			{
				s << k.kernel[i] << " ";

				//if ((i+1) % k._height == 0)
				//	s << '\n';
				
			}

			s << " )";

			return s;
		}


		
	}
};


class Sharpen : public Filter
{
public:

	Sharpen(const int & size = 3);
};

class Sharpen9 : public Filter 
{
public:

	Sharpen9(const int & size = 3);
	//void rebuild(const int & size = 3) override;
};



class Emboss : public Filter
{
public:

	Emboss(const int & size = 3);


};

class BoxBlur : public Filter
{
public:

	BoxBlur(const int & size = 3);


};

class MotionBlur : public Filter
{
public:

	MotionBlur(const int & size = 3);
	

};


class SmoothBlur : public Filter
{
public:
	SmoothBlur(const int & size = 3);

};

class UnsharpMask : public Filter {

public:

	UnsharpMask(const int & size = 5);

};

class Laplacian : public Filter {

public:

	Laplacian(const int & size = 3);
};

class GaussianBlur : public Filter {

public:

	GaussianBlur(const int & size = 5)
		:
		Filter(size)
	{

		if (size == 3)
		{
			kernel[0] = 1; kernel[1] = 2; kernel[2] = 1;
			kernel[3] = 2; kernel[4] = 4; kernel[5] = 2;
			kernel[6] = 1; kernel[7] = 2; kernel[8] = 1;

			divisor = 16.f;
		}
		else
		{
			kernel[0] = 1; kernel[1] = 4; kernel[2] = 6;  kernel[3] = 4;  kernel[4] = 1;
			kernel[5] = 4; kernel[6] = 16; kernel[7] = 24;  kernel[8] = 16;  kernel[9] = 4;
			kernel[10] = 6; kernel[11] = 24; kernel[12] = 36; kernel[13] = 24; kernel[14] = 6;
			kernel[15] = 4; kernel[16] = 16; kernel[17] = 24; kernel[18] = 16;  kernel[19] = 4;
			kernel[20] = 1; kernel[21] = 4; kernel[22] = 6;  kernel[23] = 4;  kernel[24] = 1;

			divisor = 256.f;
		}

		//_type = KernelType::Emboss;



	}

};


class DifferenceOperator : public Filter
{
protected:

	float * kernely = nullptr;

public:

	DifferenceOperator(const int & size = 3);
	~DifferenceOperator();
	//virtual void rebuild(const int & size = 3) = 0;
	virtual void apply(Image * src, Image * dst);
	virtual void apply(Image * src, Image * dst, const int & channels = 1, bool dotted = false);
	std::string toString() override;

};

class Sobel : public DifferenceOperator
{
public:

	Sobel(const int & size = 3);
	//void rebuild(const int & size = 3) override {}
};

class Scharr : public DifferenceOperator
{
public:
	Scharr(const int & size = 3);
	//void rebuild(const int & size = 3) override {}
};

class EdgeExtractor : public DifferenceOperator
{
public:

	EdgeExtractor(const int & size = 5);

	//void rebuild(const int & size = 3) override {}
};
/*{
_kernel = new float[9];

_kernel[0] = -1; _kernel[1] = -2; _kernel[2] = -1;
_kernel[3] = 0; _kernel[4] = 0; _kernel[5] = 0;
_kernel[6] = 1; _kernel[7] = 2; _kernel[8] = 1;

//_type = KernelType::Emboss;

divisor = 0.5f;
}*/






#endif