


#ifndef _COLOR_H_
#define _COLOR_H_

#include <string>
#include <assert.h>

class vec4;
class vec3;

struct Color {


	// Data
	float r, g, b, a;

	// Constructors

	Color();
	Color(float gray, float alpha = 1.0);
	Color(float R, float G, float B, float A = 1.0f);
	Color(const vec3 & vec, const float & alpha = 1.0f);
	Color(const vec4 & vec);

	// Functions

	void set(float gray);
	void set(float r, float g, float b, float a = 1.0f);
	void setRGB(int r, int g, int b, int a = 255);
	const float * ptr() const;

	// convert to 32 bits color format
	unsigned char * toColor32() const;
	std::string toString();


	// Operators

	bool operator==(Color A) const;

	// Static Functions

	static Color makeFromRGBColor(int r, int g, int b, int a = 255);
	static Color random(Color A, Color B);
	static Color lerp(const Color & from, const Color & to, float t);
	
	
	// Static variables

	const static Color white;
	const static Color black;
	const static Color grey;
	const static Color clear;
	const static Color red;
	const static Color green;
	const static Color blue;
	const static Color yellow;
	const static Color cyan;
	const static Color magenta;

	Color & operator+= (Color const & other)
	{
		this->r += other.r;
		this->g += other.g;
		this->b += other.b;
		this->a += other.a;
		return *this;
	}

	float & operator[](const size_t index) {

		assert(index >= 0 && index < 4 && "Can not access Color element.");

		return (&r)[index];

	}

	const float & operator[](const size_t index) const {

		assert(index >= 0 && index < 4 && "Can not access Color element.");

		return (&r)[index];

	}

	friend Color operator/ (Color const & c, float const & value)
	{
		return Color(c.r / value, c.g / value, c.b / value, c.a / value);
	}


};

#endif