
#ifndef  _RAYTRACE_H_
#define  _RAYTRACE_H_

#include "manager\appattributes.h"
#include "raytracer/raycasthit.h"
#include "accel\BVH.h"
#include <random>

class Raytracer {

private:

	std::atomic<uint32_t> _shadowRays;
	std::atomic<uint32_t> _secondaryRays;
	std::atomic<uint32_t> _aabbTests;
	std::atomic<uint32_t> _intersectionTests;
	std::atomic<uint32_t> _triangleTests;

	Scene * _scene = nullptr;
	BVH * bvh = nullptr;
	Options * _options = nullptr;

	std::mt19937 generator;

	//Evaluate Fresnel equation (ration of reflected light for a given incident direction and surface normal) 
	void fresnel(const vec3 & i, const vec3 & n, const float & ior, float & kr);

	// slowest and traditional trace method
	bool traceList(const Ray & ray, RaycastHit & hit);

	// more speed in tracing
	bool traceAABB(const Ray & ray, RaycastHit & hit);

	float calcShadowAmount(const Ray& ray, const Light & light, const RaycastHit & hit);

	vec2 getCMJSampler(uint64_t x, uint64_t y, uint64_t nx, uint64_t ny, uint64_t permutation, std::mt19937 & generator);

	vec2 getDiscSample(uint64_t x, uint64_t y, uint64_t nx, uint64_t ny, uint64_t permutation, std::mt19937 & generator);

public:

	Raytracer() {}
	void init(Scene *, Options *);

	Color castRay(const Ray & ray, RaycastHit & primaryHit, const uint32_t & depth);
	bool trace(const Ray & ray, RaycastHit & hit);
	void countVariables();

	const uint32_t & getShadowRays() const
	{
		return _shadowRays;
	}

	const uint32_t & getSecondaryRays() const
	{
		return _secondaryRays;
	}

	const uint32_t & getAABBTests() const
	{
		return _aabbTests;
	}

	const uint32_t & getIntersectionTests() const
	{
		return _intersectionTests;
	}

	const uint32_t & getTriangleTests() const
	{
		return _triangleTests;
	}

	void clear();

};

#endif