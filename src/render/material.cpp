
#include "math/mat3.h"
#include "math/vec3.h"
#include "render/material.h"
#include "entity/primitive.h"
#include "math/maths.h"
#include "render/proceduraltexture.h"

#include <algorithm> 


//const Material Material::defaultMaterial(Color color(1, 1, 1, 1)) {_diffuse = color}; //= { Color::white, Color::black };

Material::Material() 
	
{
	_kt = vec3(1, 1, 1);
	_kd = vec3(1,1,1);
	_ks = vec3(0.5f, 0.5f, 0.5f);
	_ka = 1.0f;
	_ns = 100.f;
	_d = 1.0;
	
	_ni = 0;//0.12;
	_texture = nullptr;
	_normalMap = nullptr;
	_transparentMap = nullptr;
	_pattern = nullptr;
	_unlit = false;
}


Material::Material(const vec3 & diffuse, const vec3 & specular, float shininess, float refraction) 
	:
	_kd(diffuse),
	_ks(specular),
	_ns(shininess),
	_ni(refraction)
{
	
	_kd = 1.0f;
	_ks = 0.2f;
	_ka = 1.0f;
	_ns = 100.f;
	_d = 1.0;
	_kt = vec3(1, 1, 1);
	_ni = 0;
	_texture = nullptr;
	_normalMap = nullptr;
	_transparentMap = nullptr;
	_pattern = nullptr;
	_unlit = false;
}

Material::~Material()
{
	//_texture = nullptr;
	//_bumpTexture = nullptr;
}


void Material::shade(const Ray & ray, const RaycastHit & hit, const vec3 & lightColor, const vec3 & lightDir, vec3 & diffuseContrib, vec3 & specularContrib) 
{
	//vec3 color;
	vec3 albedo;
	vec3 normal = hit.normal;
	vec3 ldir = -lightDir;
	// check texture

	if (_texture != nullptr && _texture->loaded())
		albedo = _texture->getTexel(hit.texCoord);
	else if (_pattern != nullptr) {
		if (_pattern->kind == 1)
			albedo = ProceduralTexture::stripe(hit.texCoord, _pattern->angle, _pattern->scale);
		else
			albedo = ProceduralTexture::checkboard(hit.texCoord, _pattern->a, _pattern->b, _pattern->scale);
	}
	else
		albedo = _kd;
	
	if (!_unlit) {

	
		if (_normalMap != nullptr && _normalMap->loaded())
		{
			const mat4 & m = hit.object->getInversedMatrix();
			vec3 wt = vec3(m * vec4(hit.tangent, 1));
			normal = calcBumpedNormal(wt, normal, hit.texCoord);
		}

		if (albedo.maxComponent() > Epsilon)
			diffuseContrib += albedo * lightColor * std::max(0.f, vec3::dot(normal, ldir));

		if (this->_ks.maxComponent() > Epsilon && lightColor > Epsilon) {
			//(lightDir.normalized() + -1 * ray.direction.normalized()).normalized();
			vec3 r = vec3::reflect(lightDir, hit.normal);//(-lightDir + (-1 * ray.direction)).normalized();
			specularContrib += this->_ks *  lightColor * pow(std::max(0.f, vec3::dot(r, -ray.direction)), _ns); //std::max(0.f, ); // 
		}

		// transparent map
		if (_transparentMap != nullptr && _transparentMap->loaded())
			diffuseContrib += _transparentMap->getTexel(hit.texCoord) * lightColor * std::max(0.f, vec3::dot(hit.normal, ldir)) * _transparentMapIntensity;

	}
	else {
		diffuseContrib = albedo;
	}

	//return color;

	/*if (_normalMap != nullptr && _normalMap->loaded())
	{
		vec3 bumpNormal;
		// calculate bi-tangent // tangent space normal mapping
		vec3 bitanget = vec3::cross(hit.tangent, normal).normalized();

		mat4 a = hit.object->getMatrix();

		// TBN matrix
		//mat3 TBN((a * hit.tangent), (a * bitanget), normal);

		mat3 TBN(hit.tangent, bitanget, normal);
		TBN = mat3::transpose(TBN);

		// Perturbed Normal - RGB
		bumpNormal = _normalMap->getTexel(hit.texCoord);
		bumpNormal = 2.0f * (bumpNormal - 0.5f);
		//bumpNormal.normalized();
		normal = (TBN * bumpNormal).normalized();
		ldir = (TBN * -lightDir).normalized();
		//ldir *= -1;
	}*/
	

/*	bool isBack = (vec3::dot(hit.normal, ray.direction) > 0);
	if (isBack)
	{
		// std::cout << "Back face: " << Vector3f::dot(normal, rayDir) << endl;
		normal *= -1; // Flip normal
	}*/


	// is not unlit, then compute difusse and specular
/*	if (!_unlit) {
	


		//color = pointWiseDot(lightColor, kd) * clampedDot(hit.normal, -lightDir);

		//std::cout << vec3::dot(normal, ldir) << std::endl;

		// lambert
		if (albedo.max() > fEpsilon)
			diffuseContrib += albedo * lightColor * std::max(0.f, vec3::dot(normal, ldir));
		
	

		// specular - phong

		if (this->_specularColor.max() > fEpsilon && lightColor > fEpsilon) {
			//(lightDir.normalized() + -1 * ray.direction.normalized()).normalized();
			vec3 r = vec3::reflect(lightDir, hit.normal);//(-lightDir + (-1 * ray.direction)).normalized();
			specularContrib +=   lightColor * pow(std::max(0.f, vec3::dot(r, -ray.direction)), _ns); //std::max(0.f, ); // 
		}
		//color += specularColor;

	}
	else 
		diffuseContrib = albedo;*/
	

	//return color;

	

}

vec3 Material::shade2(const Ray & ray, const RaycastHit & hit, const vec3 & lightColor, const vec3 & lightDir) {
	

	vec3 normal = hit.normal;
	vec3 albedo;
	vec3 color;
	vec3 ldir = -lightDir;

	// check texture
	if (_texture != nullptr && _texture->loaded())
		albedo = _texture->getTexel(hit.texCoord);
	else if (_pattern != nullptr) {
		if (_pattern->kind == 1)
			albedo = ProceduralTexture::stripe(hit.texCoord, _pattern->angle, _pattern->scale);
		else 
			albedo = ProceduralTexture::checkboard(hit.texCoord,_pattern->a, _pattern->b, _pattern->scale);
	}
	else
		albedo = _kd;


	/*bool isBack = (vec3::dot(hit.normal, ray.direction) > 0);
	if (isBack)
	{
		// std::cout << "Back face: " << Vector3f::dot(normal, rayDir) << endl;
		normal *= -1; // Flip normal
	}*/

	// is not unlit, then compute difusse and specular
	if (!_unlit) {
		if (_normalMap != nullptr && _normalMap->loaded())
		{
			const mat4 & m = hit.object->getInversedMatrix();
			vec3 wt = vec3(m * vec4(hit.tangent, 1));
			normal = calcBumpedNormal(wt, normal, hit.texCoord);
		}

		

			// lambert
		if (albedo.maxComponent() > Epsilon)
				color = albedo * lightColor * std::max(0.f, vec3::dot(normal, ldir));

		// specular - phong

		if (this->_ks.maxComponent() > Epsilon && lightColor > Epsilon) {
			//(lightDir.normalized() + -1 * ray.direction.normalized()).normalized();
			vec3 r = vec3::reflect(-lightDir, normal);//(-lightDir + (-1 * ray.direction)).normalized();
			color += this->_ks * lightColor * pow(std::max(0.f, -vec3::dot(r, ray.direction)), _ns); //std::max(0.f, ); // 
		}

		// transparent map
		if (_transparentMap != nullptr && _transparentMap->loaded())
			color += _transparentMap->getTexel(hit.texCoord) * lightColor * std::max(0.f, vec3::dot(hit.normal, ldir)) * _transparentMapIntensity;

	} else {

		if (_transparentMap != nullptr && _transparentMap->loaded())
			color += _transparentMap->getTexel(hit.texCoord);
	}


	return color;

}


vec3 Material::shadeCelShading(const Ray & ray, const RaycastHit & hit, const vec3 & lightColor, const vec3 & lightDir, const float & levels, const float & factor, const bool & outline)
{
	vec3 normal = hit.normal;
	vec3 albedo;
	vec3 color;
	vec3 ldir = -lightDir;

	// check texture
	if (_texture != nullptr && _texture->loaded())
		albedo = _texture->getTexel(hit.texCoord);
	else if (_pattern != nullptr) {
		if (_pattern->kind == 1)
			albedo = ProceduralTexture::stripe(hit.texCoord, _pattern->angle, _pattern->scale);
		else
			albedo = ProceduralTexture::checkboard(hit.texCoord, _pattern->a, _pattern->b, _pattern->scale);
	}
	else
		albedo = _kd;


	/*bool isBack = (vec3::dot(hit.normal, ray.direction) > 0);
	if (isBack)
	{
	// std::cout << "Back face: " << Vector3f::dot(normal, rayDir) << endl;
	normal *= -1; // Flip normal
	}*/

	// is not unlit, then compute difusse and specular
	if (!_unlit) {
		if (_normalMap != nullptr && _normalMap->loaded())
		{
			const mat4 & m = hit.object->getInversedMatrix();
			vec3 wt = vec3(m * vec4(hit.tangent, 1));
			normal = calcBumpedNormal(wt, normal, hit.texCoord);
		}



		// lambert
		vec3 lambert;
		vec3 difus;
	
		// difusa
		// albedo é a cor da difusa
		// lambert é dividido em numero de niveis do cell shading
		if (albedo.maxComponent() > Epsilon) {
			lambert = floorf(std::max(0.f, vec3::dot(normal, ldir)) * levels) * factor;
			difus = albedo * lightColor * lambert;//floorf(std::max(0.f, vec3::dot(normal, ldir)) * levels) * factor;
		}

		// specular - phong
		// calcula specular
		vec3 specular;
		vec3 r = vec3::reflect(-lightDir, normal);
		if (this->_ks.maxComponent() > Epsilon && lightColor > Epsilon) {
			specular = this->_ks * lightColor * std::max(0.f, -vec3::dot(r, ray.direction));// pow(, _ns);
			// can use clamp to create a border
			// pow(Math::clamp(-vec3::dot(r, ray.direction), 0.f, 1.0f), _ns);
		}

		// limit specular
		vec3 h = (ldir + ray.direction).normalized();
		float specMask = (std::powf(vec3::dot(h, normal), _ns) > 0.5) ? 1 : 0;
		//float specMask = (std::powf(vec3::dot(h, ray.direction), _ns) > 0.4) ? 1 : 0;

		//edege
		float edgeDetection = 1;

		if (outline)
			edgeDetection = (vec3::dot(normal, ray.direction) > 0.35) ? 1 : 0;

		color = edgeDetection * (difus + specular * specMask);

		// transparent map
		if (_transparentMap != nullptr && _transparentMap->loaded())
			color += _transparentMap->getTexel(hit.texCoord) * lambert * _transparentMapIntensity;

	}
	else {

		if (_transparentMap != nullptr && _transparentMap->loaded())
			color += _transparentMap->getTexel(hit.texCoord);
	}


	return color;
}

vec3 Material::shadeGooch(const Ray & ray, const RaycastHit & hit, const vec3 & lightColor, const vec3 & lightDir, 
	const vec3 & warmColor, const vec3 & coolColor, const float & alpha, const float & beta,
	const bool & outline, bool superCut)
{
	vec3 albedo;
	vec3 color;
	vec3 normal = hit.normal;
	vec3 ldir = -lightDir;

	// check texture
	if (_texture != nullptr && _texture->loaded())
		albedo = _texture->getTexel(hit.texCoord);
	else if (_pattern != nullptr) {
		if (_pattern->kind == 1)
			albedo = ProceduralTexture::stripe(hit.texCoord, _pattern->angle, _pattern->scale);
		else
			albedo = ProceduralTexture::checkboard(hit.texCoord, _pattern->a, _pattern->b, _pattern->scale);
	}
	else
		albedo = _kd;


	if (!_unlit) {
		if (_normalMap != nullptr && _normalMap->loaded())
		{
			const mat4 & m = hit.object->getInversedMatrix();
			vec3 wt = vec3(m * vec4(hit.tangent, 1));
			normal = calcBumpedNormal(wt, normal, hit.texCoord);
		}

		// intensity of diffuse lighting [-1, 1]
		float diffuseLighting = vec3::dot(normal, ldir);
		
		
		// lambert		
		vec3 difus;

		if (albedo.maxComponent() > Epsilon)
		{
			if (!superCut)
			{
				// cool color mixed with color of the object
				vec3 coolColorMod = coolColor + albedo * alpha;// u_alpha;
				// warm color mixed with color of the object
				vec3 warmColorMod = warmColor + albedo * beta; // u_beta;


				// map intensity of lighting from range [-1; 1] to [0, 1]
				float intensityMap = (1.0 + diffuseLighting) / 2;

				// interpolation of cool and warm colors according 
				// to lighting intensity. The lower the light intensity,
				// the larger part of the cool color is used
				//
				difus = lightColor * vec3::lerp(coolColorMod, warmColorMod, intensityMap);
			}
			else
			{	
				// cool color mixed with color of the object
				vec3 coolColorMod = albedo * alpha;// u_alpha;
				// warm color mixed with color of the object
				vec3 warmColorMod = albedo * beta; // u_beta;

				// hi mask
				float hiMask = (diffuseLighting > 0.4) ? 1 : 0;
				//((1 + normal dot light) / 2) * color.cool + (1 - ((1 + normal dot light) / 2)) * color.warm
				//vec3 t = (intensityMap * coolColorMod) + ((1-intensityMap) * warmColorMod);
				difus = lightColor * vec3::lerp(coolColorMod, warmColorMod, hiMask);
			}

			
		}// * std::max(0.f, vec3::dot(normal, ldir));

		// specular - phong
		vec3 r = vec3::reflect(-lightDir, normal);
		vec3 spec;
		if (this->_ks.maxComponent() > Epsilon && lightColor > Epsilon) {
			if (!superCut)
			{
				//pow(std::max(0.f, -vec3::dot(r, ray.direction)), _ns);
				vec3 h = (ldir + ray.direction).normalized(); 
				spec = this->_ks * lightColor * std::powf(std::max(0.f, -vec3::dot(r, ray.direction)), _ns); // -vec3::dot(r, ray.direction) // vec3::dot(h, normal)
			}
			else
			{
				spec = this->_ks * lightColor * std::max(0.f, -vec3::dot(r, ray.direction)); // pow(), _ns);
				vec3 h = (ldir + ray.direction).normalized();

				// limit specular
				float specMask = (std::powf(vec3::dot(h, normal), _ns) > 0.5) ? 1 : 0;
				spec *= specMask;
				
			}
		}

		// edege mask
		float edgeMask = 1; 

		if (outline)
			edgeMask = (vec3::dot(normal, ray.direction) > 0.3) ? 1 : 0;

		
		 //(std::powf(vec3::dot(r, ray.direction), _ns) > 0.5) ? 1 : 0;

		color = edgeMask * (difus + spec);

		// transparent map
		if (_transparentMap != nullptr && _transparentMap->loaded())
			color += _transparentMap->getTexel(hit.texCoord) * lightColor * std::max(0.f, vec3::dot(hit.normal, ldir)) * _transparentMapIntensity;
	} 

	return color;

}

vec3 Material::calcPhong(const vec3 & diffuseContrib, const vec3 & specularContrib, const vec3 & shadowContrib)
{
	return shadowContrib *_kd + specularContrib * _ks;
}

vec3 Material::calcBumpedNormal(const vec3 & tangent, const vec3 & normal, const vec2 & texCoord)
{

	
	
	// compute tan
	vec3 tan = tangent.normalized();

	//  Gram-Schmidt process 
	tan = (tan - vec3::dot(tan, normal) * normal);
	tan.normalize();

	// calculate bi-tangent // tangent space normal mapping
	vec3 bitanget = vec3::cross(tan, normal);

		// Perturbed Normal - RGB
		vec3 bumpNormal = _normalMap->getTexel(texCoord);
		bumpNormal = 2.0f * bumpNormal - 1.0f; //( - 0.5f);
		//bumpNormal.normalize();
	
			//mat4 a = hit.object->getMatrix();

		//mat4 a = object->getMatrix();
		//mat3 TBN((a * hit.tangent), (a * bitanget), normal);

		// TBN matrix
		mat3 TBN(tan, bitanget, normal);
		//TBN = mat3::transpose(TBN);

		vec3 newNormal = TBN * bumpNormal;

		newNormal.normalize();

		return newNormal;
	
}

void Material::setPattern(Pattern * pattern) 
{
	this->_pattern = pattern;
}

void Material::setProperty(const int materialPropertyIndex, const vec3 & value)
{
	switch (materialPropertyIndex)
	{
	default:
		break;
	case MAT_AMBIENT: {
		_ka = value;
		break;
	}
	case MAT_DIFFUSE: {
		_kd  = value;
		break;
	}
	case MAT_SPECULAR: {
		_ks = value;
		break;
	}
	case MAT_TRANSMITTANCE: {
		_kt = value;
		break;
	}
	case MAT_EMISSION: {
		_ke = value;
		break;
	}
	/*case MAT_TRANSPARENT: {
		_reflective = value;
		break;
	}*/
	}
}

void Material::setProperty(const int materialPropertyIndex, const Color & value)
{
	setProperty(materialPropertyIndex, vec3(value));
}

void Material::setProperty(const int materialPropertyIndex, float value)
{
	switch (materialPropertyIndex)
	{
	default:
		break;
	case MAT_TRANSPARENCY: {
		_d = value;
		break;
	}
	case MAT_REFRACTION: {
		_ni = value;
		break;
	}
	case MAT_SHININESS: {
		_ns = value;
		break;
	}
	case MAT_ILLUM: {
		_illum = (int)value;
		break;
	}
	}
}

/*void Material::setProperty(const int materialPropertyIndex, int value)
{
	if (materialPropertyIndex == MAT_TEXTURE)
		_textureID = value;
}*/



void Material::setTexture( Texture * texture)
{
	_texture = texture;
}

void Material::setNormalMap(Texture * texture)
{
	_normalMap = texture;
}

void Material::setTransparentMap(Texture * texture)
{

	_transparentMap = texture;
	
}


void Material::setUnlit(bool flag) {

	_unlit = flag;

}

Color Material::getColor(const int materialPropertyIndex) const {

	return getVec3(materialPropertyIndex).toColor();

}

vec3 Material::getVec3(const int materialPropertyIndex) const 
{
	switch (materialPropertyIndex)
	{
	default:
		return vec3::zero;
		break;
	case MAT_AMBIENT: {
		return _ka;
		break;
	}
	case MAT_DIFFUSE: {
		return _kd;
		break;
	}
	case MAT_SPECULAR: {
		return _ks;
		break;
	}
	case MAT_TRANSMITTANCE: {
		return _kt;
		break;
	}
	case MAT_EMISSION: {
		return _ke;
		break;
	}
	}
}

float Material::getFloat(const int materialPropertyIndex) const 
{
	switch (materialPropertyIndex)
	{
	default:
		return 0;
		break;
	case MAT_TRANSPARENCY: {
		return _d;
		break;
	}
	case MAT_REFRACTION: {
		return _ni;
		break;
	}
	case MAT_SHININESS: {
		return _ns;
		break;
	}
	}
}

bool Material::getUnlit() const
{
	return _unlit;
}

Color Material::getAlbedo(const vec2 & texCoord) const
{
	if (_texture != nullptr && _texture->loaded()) {
		return _texture->getTexel(texCoord);

	}
	else if (_pattern != nullptr) {
		if (_pattern->kind == 1)
			return ProceduralTexture::stripe(texCoord, _pattern->angle, _pattern->scale);
		else
			return ProceduralTexture::checkboard(texCoord, _pattern->a, _pattern->b, _pattern->scale,_pattern->angle);
	}
	else
		return _kd;

	/*float scale = 5;
	float pattern = (fmodf(texCoord.x * scale, 1) > 0.5) ^ (fmodf(texCoord.y * scale, 1) > 0.5);
	return vec3::lerp(vec3(0.815, 0.235, 0.031), vec3(0.937, 0.937, 0.231), pattern);*/
}

/*int Material::getInteger(const int materialPropertyIndex) const 
{
	if (materialPropertyIndex == MAT_TEXTURE)
		return _textureID;
	
	return 0;
	
}*/

Texture * Material::getTexture() const
{
	return _texture;
}

Texture * Material::getNormalMap() const
{
	return _normalMap;
}
/*
vec3 Material::clampedDot(const vec3 & v1, const vec3 & v2)
{
	float d = vec3::dot(v1, v2);
	return (d>0) ? d : 0;
}

vec3 Material::pointWiseDot(const vec3 & v1, const vec3 & v2)
{
	return vec3(v1[0] * v2[0], v1[1] * v2[1], v1[2] * v2[2]);


}*/