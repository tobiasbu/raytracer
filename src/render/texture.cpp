
#include "render/texture.h"
#include "math\maths.h"

Texture::Texture()
	:
	_loaded(false),
	_image(nullptr),
	_interpolation(TextureInterpolation::Billinear)
{
	_loaded = false;
}

Texture::~Texture()
{
	if (_image != nullptr)
		delete _image;
}

bool Texture::load(const std::string & filename)
{
	if (!_loaded) {
		_image = new Image();
		if (!_image->load(filename)) {
			delete _image;
			_image = nullptr;
			_loaded = false;
		}
		else
			_loaded = true;

		// just another test
		//_image->save("test.png");
		
	} 
	return _loaded;
	

}

bool Texture::loaded() 
{
	return _loaded;
}

void Texture::setInterpolation(const TextureInterpolation & interpolation)
{
	_interpolation = interpolation;
}

void Texture::setWrap(const TextureWrap & wrap)
{
	_wrap = wrap;
}

unsigned char * Texture::getPixelAt(int x, int y)
{
	x = Math::clampint(x, 0, _image->getWidth() - 1);
	y = Math::clampint(y, 0, _image->getHeight() - 1);

	return _image->getPixel(x,y);
}

void Texture::getPixel(int x, int y, unsigned char * pixels)
{

	unsigned char * px = getPixelAt(x, y);
	pixels[0] = px[0];
	pixels[1] = px[1];
	pixels[2] = px[2];
	//pixels[3] = px[3];
}

Color Texture::getPixelColor(int x, int y)
{
	x = Math::clampint(x, 0, _image->getWidth() - 1);
	y = Math::clampint(y, 0, _image->getHeight() - 1);

	return _image->getPixelColor(x, y);
}

Color Texture::getTexel(float x, float y)
{
	Color color;
	int px, py;

	if (_wrap == TextureWrap::Repeat)
	{
		x = std::fmodf(x, 1.0);
		y = std::fmodf(y, 1.0);
	}

	// ratio_x
	x = (x * _image->getWidth()); // (1-x)

	// ratio_y
	y = ((1 - y) * _image->getHeight()); //  inverted (1 - y)

	// pixels coordinates
	px = (int)x;
	py = (int)y;




	if (_interpolation == TextureInterpolation::Point) 
	{
		color = getPixelColor(px, py);
	}
	else if (_interpolation == TextureInterpolation::Billinear)
	{
		
		/*
		// METHOD WITH RGB COLOR PIXELS (unsigned char)
		// can improve to get alpha value to

		unsigned char pixels[4][3];

		// pixels
		getPixel(px, py, pixels[0]);
		getPixel(px + 1, py, pixels[1]);
		getPixel(px, py + 1, pixels[2]);
		getPixel(px + 1, py + 1, pixels[3]);

		float alpha = x - (float)px;
		float beta = y - (float)py;


		for (int i = 0; i < 3; i++) {
			color[i] = 
				(1 - alpha)*(1 - beta)*pixels[0][i]
				+ alpha *(1 - beta)*pixels[1][i]
				+ (1 - alpha)*   beta *pixels[2][i]
				+ alpha *   beta *pixels[3][i];
		}

		color.r /= 255.0;
		color.g /= 255.0;
		color.b /= 255.0;
		//color.a /= 255.0;
		*/

		///////////////////////////////////////////////

		// METHOD WITH RGB COLOR NORMALIZED (float)

		
		Color c00 = getPixelColor(px, py);
		Color c10 = getPixelColor(px+1, py);
		Color c01 = getPixelColor(px, py+1);
		Color c11 = getPixelColor(px+1, py+1);

		// opposite x
		float tx = x - px;

		// opposite y
		float ty = y - py;

		for (int i = 0; i < 3; i++)
		{
		color[i] = Math::blerp(c00[i], c10[i], c01[i], c11[i], tx, ty);

		/*color[i] =
		(1 - tx) * (1 - ty) * c00[i]
		+ tx * (1 - ty) * c10[i]
		+ (1 - tx)* ty * c01[i]
		+ tx * ty * c11[i];*/


		}
		

		///////////////////////////////////////////////
		// Computing Alpha:
		// For raytracer its not necessary? Maybe...
		// But it will be needed a bleeding function to compute alpha.... :-(
		//
		///// Backup Formula: alph = (t(a)  * x_opposite + t(be)   * x_ratio) * y_opposite + (t(c) * x_opposite + t(d) * x_ratio) * y_ratio;
		//
		// - Compute the alpha:
		// float alpha = (c00.a  * tx + c10.a * x_ratio) * ty + (c01.a * tx + c11.a * x_ratio) * y_ratio;
		//
		// - Then for each component: 
		// red = 
		// (c00.r * c00.a * x_opposite + c10.r * c10.a * x_ratio) * ty + (c01.r * c01.a * x_opposite + c11.r * c11.a * x_ratio) * y_ratio); ...
		//
		// As Mark Ransom say in this topic:
		// "You need to multiply each of your r,g,b values by the corresponding alpha before working with them,
		//  then divide the values by the final alpha when you're done. [...]
		//  the effect this will have when one of the pixels has an alpha of zero - each of the r,g,b values will be multiplied by zero!!
		//  So it won't matter what their original values are at all! They won't contribute to the final result."
		//
		// Check here: http://stackoverflow.com/questions/347294/adding-alpha-values-to-bilinear-resizing-algorithm
		///////////////////////////////////////////////


		// Let's set alpha to 1;
		color.a = 1;
	}

	return color;

	/*Color color;
	int ix, iy;
	x = x*_image->getWidth();
	y = (1 - y)*_image->getHeight();
	ix = (int)x;
	iy = (int)y;

	unsigned char pixels[4][3];

	float alpha = x - ix;
	float beta = y - iy;

	getPixel(ix, iy, pixels[0]);
	getPixel(ix+1, iy, pixels[1]);
	getPixel(ix, iy+1, pixels[2]);
	getPixel(ix+1, iy+1, pixels[3]);
	//pixels[0] = getPixel(ix, iy);
	//(*pixels[1]) = *getPixel(ix + 1, iy);
	//(*pixels[2]) = *getPixel(ix, iy + 1);
	//(*pixels[3]) = *getPixel(ix + 1, iy + 1);
	//getPixel(ix + 1, iy, pixels[1]);
	//getPixel(ix, iy + 1, pixels[2]);
	//getPixel(ix + 1, iy + 1, pixels[3]);

	for (int i = 0; i < 3; i++) {
		color[i] = (1 - alpha)*(1 - beta)*pixels[0][i]
			+ alpha *(1 - beta)*pixels[1][i]
			+ (1 - alpha)*   beta *pixels[2][i]
			+ alpha *   beta *pixels[3][i];
	}
	color.r /= 255.0;
	color.g /= 255.0;
	color.b /= 255.0;
	//color.a /= 255.0;
	return color;*/
}

Color Texture::getTexel(vec2 texCoord)
{
	return getTexel(texCoord.x, texCoord.y);
	//return _image->getPixel(x, y);
}