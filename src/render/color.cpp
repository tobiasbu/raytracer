
#include "render\color.h"
#include "math/vec4.h"
#include "math/vec3.h"
#include "math/maths.h"
#include <sstream>

const Color Color::white(1, 1, 1, 1);
const Color Color::grey(0.5f, 0.5f, 0.5f, 1);
const Color Color::black(0, 0, 0, 1);
const Color Color::clear(0.f, 0.f, 0.f, 0.f);
const Color Color::red(1, 0, 0, 1);
const Color Color::green(0, 1, 0, 1);
const Color Color::blue(0, 0, 1, 1);
const Color Color::yellow(1, 1, 0, 1);
const Color Color::cyan(0, 1, 1, 1);
const Color Color::magenta(1, 0, 1, 1);

Color::Color() :
r(1.0),
g(1.0),
b(1.0),
a(1.0)
{
}

Color::Color(float R, float G, float B, float A) :
r(R),
g(G),
b(B),
a(A)
{
	/*	this->r = (float)r;
	this->g = (float)g;
	this->b = (float)b;
	this->a = (float)a;*/

	//return this;
}

Color::Color(float gray, float alpha) :
r(gray),
g(gray),
b(gray),
a(alpha) {

}

Color::Color(const vec4 & v)
	:
	r(v.x),
	g(v.y),
	b(v.z),
	a(v.w)
{

}

Color::Color(const vec3 & v, const float & alpha)
	:
	r(v.x),
	g(v.y),
	b(v.z),
	a(alpha)
{

}

void Color::set(float gray) {
	this->r = gray;
	this->g = gray;
	this->b = gray;
}

void Color::set(float r, float g, float b, float a) {
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

void Color::setRGB(int r, int g, int b, int a) {

	this->r = (float)r / 255.0f;
	this->g = (float)g / 255.0f;
	this->b = (float)b / 255.0f;
	this->a = (float)a / 255.0f;

}

const float * Color::ptr() const
{
	return &r;
}

unsigned char * Color::toColor32() const {

	unsigned char rgba[4];
	rgba[0] = unsigned char(Math::clamp(r, 0.00000001f, 1.000000f) * 255.f);
	rgba[1] = unsigned char(Math::clamp(g, 0.00000001f, 1.000000f) * 255.f);
	rgba[2] = unsigned char(Math::clamp(b, 0.00000001f, 1.000000f) * 255.f);
	rgba[3] = unsigned char(Math::clamp(a, 0.00000001f, 1.000000f) * 255.f);

	// (unsigned char)((float)fabs(a*255) + 0.5f);

	return rgba;

}

std::string Color::toString() {

	std::stringstream str;
	str << r << ", " << g << ", " << b << ", " << a;
	return str.str();

}

Color Color::makeFromRGBColor(int r, int g, int b, int a) {

	Color color;
	color.r = (float)r / 255.0f;
	color.g = (float)g / 255.0f;
	color.b = (float)b / 255.0f;
	color.a = (float)a / 255.0f;

	return color;

}

Color Color::lerp(const Color & from, const Color & to, float t) {

	Color l;
	l.r = Math::lerp(from.r, to.r, t);
	l.g = Math::lerp(from.g, to.g, t);
	l.b = Math::lerp(from.b, to.b, t);
	l.a = Math::lerp(from.a, to.a, t);

	return l;

}

bool Color::operator==(Color other) const
{
	if (other.r == r && other.g == g &&	other.b == b &&	other.a == a)
		return true;
	else
		return false;
}