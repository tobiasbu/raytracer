
#ifndef _RAYCASTRENDER_H_
#define _RAYCASTRENDER_H_

#include "render\render.h"

#include <mutex>

struct vec2i
{
	int x, y;
	vec2i(int x, int y)
		:
		x(x),
		y(y)
	{

	}
};

class RaycastRender : public Render {

private:

	std::atomic<uint32_t> _primaryRays;

	std::vector<std::pair<vec2i, float>> distances;
	float tmin = 0, tmax = 0;
	std::mutex mtx;
	
	// calculate thread image boundaries
	std::vector<int> threadBounds(const int & nthreads, const int & with, const int & height);
	void parallelRay(Image * out, const vec3 & origin, int left, int right);
	void parallelRayEachLine(Image * out, const vec3 & origin, int index, int start, int end);
	void computeSobelEdges();

public:

	RaycastRender() {}
	void reset();
	void end();
	Image * draw() override;

	Image * drawMultiThread(const int & nthreads, bool screenSubDivision = true) override;

	const uint32_t & getPrimaryRays() const
	{
		return _primaryRays;
	}

	void clear()
	{
		if (_raytracer || _raytracer != nullptr || _raytracer != NULL)
			_raytracer->clear();

		//delete _raytracer;
		//_raytracer = nullptr;

		//_hasBeenInitialized = false;
	}


};

#endif