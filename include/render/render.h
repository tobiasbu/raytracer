
#ifndef   _RENDER_H_
#define   _RENDER_H_

#include "image\image.h"
#include "manager\projection.h"
#include "entity\camera.h"
#include "entity\scene.h"
#include "manager\appattributes.h"
#include "raytracer\raytracer.h"

class Render {

protected:


	Raytracer * _raytracer;
	Scene * _scene;
	Options * _options;
	Camera * _camera;
	int _width, _height;
	float _fov_x;
	float _fov_y;
	float _scale;
	float _aspectRatio;
	mat4 _view;
	float _minT;
	float _maxT;
	bool _hasBeenInitialized = false;
	//Camera * _camera;
	//Projection projection;
	//Scene & scene;
	//std::vector<Primitive *> * _objects;

	
	Image * output = nullptr;
	Image * depth = nullptr;
	Image * normal = nullptr;
	Image * edges = nullptr;

public:

	Render() {}

	void initialize(AppAttributes * app);
	const Raytracer * getRaytracer() const
	{
		return _raytracer;
	}
	
	virtual Image * draw() = 0;
	virtual Image * drawMultiThread(const int & nthreads, bool screenSubDivision = true) = 0;

	Image * getDepthImage() const
	{
		return depth;
	}

	Image * getNormalImage() const
	{
		return normal;
	}

	Image * getEdgesImage() const
	{
		return edges;
	}

	//void setScene();
	//bool rayCast(Ray ray, RaycastHit * hit);

};

#endif