
#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include <string>

#include "raytracer/ray.h"
#include "raytracer/raycasthit.h"
#include "render/color.h"
#include "render/texture.h"

#define MAT_AMBIENT 0x0
#define MAT_DIFFUSE 0x1
#define MAT_SPECULAR 0x2
#define MAT_TRANSMITTANCE 0x3
#define MAT_TRANSPARENCY 0x4
#define MAT_REFRACTION 0x5
#define MAT_SHININESS 0x6
#define MAT_EMISSION 0x7
#define MAT_ILLUM 0x8

#define MAT_TEXTURE_DIFFUSE 0x9
#define MAT_TEXTURE_SPECULAR 0x10
#define MAT_TEXTURE_NORMAL 0x11
#define MAT_TEXTURE_TRANSPARENT 0x12

// TODO better pattern struct
struct Pattern 
{
	short kind;
	vec2 scale;
	float angle;
	Color a;
	Color b;
	Pattern() 
	{ 
		kind = 0;
		a = Color::white;
		b = Color::black; 
		angle = 0; 
		scale = vec2(10, 10); 
	}
};

class Material {

private:

	// ambient color
	vec3 _ka;
	// diffuse color
	vec3 _kd;
	// specular color
	vec3 _ks;
	// transmitance - reflextive color
	vec3 _kt;
	// specular expoent
	float _ns;
	// transparency - dissolve
	float _d;
	// refraction index - ior
	float _ni;
	// emission
	vec3 _ke;
	// illum
	int _illum;

	Pattern * _pattern;

	// texture map_kd
	Texture * _texture;
	// texture normalmap map_bump
	Texture * _normalMap;
	// texture transparentmap map_d
	Texture * _transparentMap;
	// intensity of transparentmap
	float _transparentMapIntensity = 0.5;


	bool _unlit = false;
	//void * noise;

	vec3 calcBumpedNormal(const vec3 & tangent, const vec3 & normal, const vec2 & texCoord);

public:




	// diffuse color = albedo
	//vec3 _color;
	// specular color
	//vec3 _specularColor;
	//vec3 _reflectiveColor;
	//vec3 _transparent;


	Material();
	Material(const vec3 & diffuse, const vec3 & specular = vec3(0.85f), float shininess = 1, float refraction = 0);

	~Material();


	void setPattern(Pattern * pattern);
	void setProperty(const int materialPropertyIndex, const vec3 & value);
	void setProperty(const int materialPropertyIndex, const Color & value);
	void setProperty(const int materialPropertyIndex, float value);
	//void setProperty(const int materialPropertyIndex, int value);
	void setTexture(Texture * texture);
	void setNormalMap(Texture * texture);
	void setTransparentMap(Texture * texture);
	void setUnlit(bool flag);

	// shading with light 
	void shade(const Ray & ray, const RaycastHit & hit, const vec3 & lightColor, const vec3 & lightDir, vec3 & diffuseContrib, vec3 & specularContrib);
	vec3 calcPhong(const vec3 & diffuseContrib, const vec3 & specularContrib, const vec3 & shadowContrib);

	// just a test
	vec3 shade2(const Ray & ray, const RaycastHit & hit, const vec3 & lightColor, const vec3 & lightDir);

	vec3 shadeCelShading(const Ray & ray, const RaycastHit & hit, const vec3 & lightColor, const vec3 & lightDir,
		const float & levels, const float & factor, const bool & outline = true);

	vec3 shadeGooch(const Ray & ray, const RaycastHit & hit, const vec3 & lightColor, const vec3 & lightDir,
		const vec3 & warmColor, const vec3 & coolColor, const float & alpha, const float & beta, 
		const bool & outline = true, bool superCut = false);

	// un-lit shading
	//vec3 shade(const Ray & ray, const RaycastHit & hit);

	vec3 getVec3(const int materialPropertyIndex) const;
	Color getColor(const int materialPropertyIndex) const;
	float getFloat(const int materialPropertyIndex) const;
	bool getUnlit() const;
	Color getAlbedo(const vec2 & texCoord = vec2(0)) const;
	//int getInteger(const int materialPropertyIndex) const;
	Texture * getTexture() const;
	Texture * getNormalMap() const;


	


};

#endif