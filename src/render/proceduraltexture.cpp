
#include "render/proceduraltexture.h"
#include "math/maths.h"

Color ProceduralTexture::checkboard(const vec2 & st, const Color & a, const Color & b, const vec2 & scale, const float & angle)
{

	//float pattern = (fmodf(st.x * scale.x, 1) > 0.5) ^ (fmodf(st.y * scale.y, 1) > 0.5);

	float ang = angle * DEG_TO_RAD;
	float s = st.x * cos(angle) - st.y * sin(angle);
	float t = st.y * cos(angle) + st.x * sin(angle);
	float pattern = (Math::modulo(s * scale.x) < 0.5f) ^ (Math::modulo(t * scale.y) < 0.5f);

	return Color::lerp(a, b, pattern);


	// vec3(0.815, 0.235, 0.031), vec3(0.937, 0.937, 0.231)
}

Color ProceduralTexture::stripe(const vec2 & st, const float & angle, const vec2 & scale) {


	//float scaleS = 20, scaleT = 20;

	float a = angle * DEG_TO_RAD;
	float s = sin(a), c = cos(a);
	float sc = st.x * c - st.y * s;
	float t = st.y * c + st.x * s;
	
	return a;
	//float pattern = (cos(hitTexCoordinates.y * 2 * M_PI * scaleT) * sin(hitTexCoordinates.x * 2 * M_PI * scaleS) + 1) * 0.5; // isect.hitPrimitive->albedo
	
	//float pattern = (Math::modulo(s * scaleS) < 0.5);
	//hitColor += vis * pattern * lightIntensity * std::max(0.f, hitNormal.dotProduct(-lightDir));

}