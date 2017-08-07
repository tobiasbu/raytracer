
#include "manager\projection.h"
#include "math/matrixtransform.h"
//#include "math/maths.h"

Projection::Projection() {

	_camera = nullptr;
	_width = 800;
	_height = 600;
	_projectionMatrix = mat4::identity;
	setPerspective(60.f, 0.03f, 1000.f);
	setViewPort(0, 0, 1, 1);

	updateProjection();
}

void Projection::reset() {
	_width = 800;
	_height = 600;
	_projectionMatrix = mat4::identity;
	setPerspective(60.f, 0.03f, 1000.f);
	setViewPort(0, 0, 1, 1);
}

Projection::Projection(Camera * camera, int width, int height) {

	_camera = camera;
	_width = width;
	_height = height;
	setPerspective(60.f,0.03f,1000.f);
	setViewPort(0, 0, 1, 1);

	updateProjection();

}

void Projection::updateProjection() {

	// set aspect ratio
	_aspectRatio = (float)_width / (float)_height;

	// set projection matrix
	_projectionMatrix = MatrixTransform::perspective(_fov, _aspectRatio ,_near, _far);

	// set bounds (unused)
	vec3 lt = vec3(-_aspectRatio * _fov,_fov, 0.003f);
	vec3 rb = vec3( _aspectRatio * _fov,_fov, 1000.f);

	if (_camera != nullptr) {
		lt += _camera->position;
		rb += _camera->position;
	}

	_bounds.set(lt,rb);

	// set check if screen is wilder, square (in unsed here)
	/*if (_width == _height) 
		_proj = 0;
	else if (_width > _height) 
		_proj = 1;
	else 
		_proj = 2;*/

	
	
}

void Projection::setViewPort(float left, float top, float right, float bottom) {

	_viewPort[0] = left;
	_viewPort[1] = top;
	_viewPort[2] = right;
	_viewPort[3] = bottom;

}

void Projection::setViewSize(int width, int height) {
	_width = width;
	_height = height;
	_aspectRatio = (float)_width / (float)_height;
}

void Projection::setCamera(Camera * camera) {

	_camera = camera;
	

}

void Projection::setPerspective(float fov, float near, float far) {

	_fov = fov; // fovy * DEG_TO_RAD;
	_near = near;
	_far = far;
}

Bounds Projection::getBounds() {

	return _bounds;

}

int Projection::getHeight() const {

	return _height;

}

int Projection::getWidth() const {

	return _width;

}

float Projection::getAspectRatio() const {

	return _aspectRatio;

}

float Projection::getFOV() const {

	return _fov;

}

const mat4 & Projection::getMatrix() const {

	return _projectionMatrix;

}
