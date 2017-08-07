
#include "entity\light.h"
#include "math/maths.h"

DirectionalLight::DirectionalLight()
{
	color = Color::white;
	position = vec3(0, -1, 0);
	intensity = 1;
	setDirection(vec3(0, 0, 1));
	type = 0;
}

DirectionalLight::DirectionalLight(const vec3 & position, const vec3 & targetDirection, const Color & color, float intensity) {

	this->color = color;
	this->position = position;
	this->intensity = intensity;
	setDirection(targetDirection);
	type = 0;
	//this->direction = direction;

}

void DirectionalLight::setDirection(const vec3 & to) {

	direction = to;// to - position;
	direction.normalize();
}

void DirectionalLight::illuminate(const vec3 & point, vec3 & lightDir, vec3 & lightColorIntensity, float & distance, float & radius) const
{
	lightDir = direction;
	lightColorIntensity = vec3(color) * intensity;
	distance = std::numeric_limits<float>::max();
	radius = Infinity;
}



PointLight::PointLight()
{
	color = Color::white;
	position = vec3(0, -1, 0);
	intensity = 1;
	type = 1;
}

PointLight::PointLight(const vec3 & position, const Color & color, float falloff) {

	this->color = color;
	this->position = position;
	this->intensity = falloff;
	type = 1;

}

void PointLight::illuminate(const vec3 & point, vec3 & lightDir, vec3 & lightColorIntensity, float & distance, float & radius) const
{
	// direction
	// the direction to the light is the opposite of the
	// direction of the directional light source
	lightDir = position - point; // (point - position); //position - po

	// distance to Light // abs = mag
	float r2 = lightDir.norm();
	distance = sqrtf(r2);

	lightDir = (lightDir / distance);

	

	vec3 col(color);

	//square falloff
	//lightColorIntensity = col * intensity / (4 * PI * r2); 

	//lightColorIntensity = col / (1 + intensity*distance*distance);

	radius = (4 * PI * r2);

	//distance = vec3::dot(lightDir, lightDir);
	lightColorIntensity = col * intensity / radius;
	// Inverse-square law:
	//lightColorIntensity = col / (1 + intensity*distance*distance);
	
}

