
#ifndef _PROCEDURALTEXTURE_H_
#define _PROCEDURALTEXTURE_H_


#include "math/vec3.h"
#include "math/vec2.h"
#include "color.h"
#include "math/maths.h"

namespace ProceduralTexture {

	Color checkboard(const vec2 & st, const Color & a = Color::black, const Color & b = Color::white, const vec2 & scale = 5, const float & angle = 0);

	Color stripe(const vec2 & st, const float & angle = 0, const vec2 & scale = 20);


}

#endif