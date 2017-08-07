
#include "render/render.h"
#include "math/maths.h"
#include <iostream>

// Scene * scene, Camera * camera, Options * options, const Projection & projection
void Render::initialize(AppAttributes * app) {

	
	_width = app->projection.getWidth();
	_height = app->projection.getHeight();
	_aspectRatio = app->projection.getAspectRatio();
	//std::cout << projection.getFOV() << " " << _aspectRatio << std::endl;
	_scale = tan(app->projection.getFOV() * DEG_TO_RAD);
	_fov_y = _scale * 0.5f; // tan(projection.getFOV()*0.5 * DEG_TO_RAD);
	_fov_x = _fov_y * _aspectRatio; //  * 0.5;
	_camera = &app->camera;
	_scene = &app->scene;
	//_view =;
	_view = _camera->getMatrix();

	_options = &app->options;

	uint8_t axis = _scene->getWorldBounds().getMaxAxis();
	_maxT = _scene->getWorldBounds().getMax()[axis];
	_minT = _scene->getWorldBounds().getMin()[axis];
	//std::cout << scene->getPrimitives()[0]->position << std::endl;

	if (!_hasBeenInitialized) {

		_raytracer = new Raytracer;

	}
	_raytracer->init(&app->scene, &app->options);

	_hasBeenInitialized = true;
}

/*bool Render::rayCast(Ray ray, RaycastHit * hit) {

	Primitive * hitPrimitive = nullptr;
	float tNear = float_infinity;
	

	for (int i = 0; i < _objects->size(); i++) {

		float t = float_infinity;
		bool intersects = (*_objects)[i]->intersects(ray, t);

		if (intersects && t < tNear) {
			//std::cout << tNear << std::endl;
			hitPrimitive = (*_objects)[i];
			tNear = t;
		}
		else
		{
			continue;
		}
	}
	
	if (hitPrimitive == nullptr) {
		if (hit != nullptr)
			delete hit;
		hit = nullptr;
		return false;
	} 
	else {

		if (hit == nullptr)
			hit = new RaycastHit(hitPrimitive);

		hit->normal = 
		hit->object = hitPrimitive;
		hit->distance = tNear;
		//std::cout << "oi" << std::endl;
		//hit->point = 

		
	}

	return (hitPrimitive != nullptr);

}*/