
#ifndef _PROJECTION_H_
#define _PROJECTION_H_

#include "math/bounds.h"
#include "entity\camera.h"
#include "math/mat4.h"


class Projection {

	int _width, _height;
	
	float _aspectRatio;


	float _viewPort[4];

	Camera * _camera;
	float _fov, _near, _far;

	mat4 _projectionMatrix;
	//float _cameraHeight, _cameraWidth;
	// int _proj;
	Bounds _bounds;


public:

	Projection();
	Projection(Camera * camera, int width, int height);

	void updateProjection();

	void setCamera(Camera *);
	void setViewPort(float left, float top, float right,float bottom);
	void setViewSize(int width, int height);
	void setPerspective(float fov, float near, float far);

	int getHeight() const;
	int getWidth() const;
	float getAspectRatio() const;
	float getFOV() const;
	float * getViewPort();
	const mat4 & getMatrix() const;
	Bounds getBounds();

	void reset();

};

#endif