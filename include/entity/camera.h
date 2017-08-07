

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "math/vec2.h"
#include "math/vec3.h"
#include "math/mat4.h"
#include "raytracer\ray.h"

// FPS CAMERA
class Camera {

private:

	mat4 _viewMatrix;
	vec3 _target;
	float _yaw;
	float _pitch;
	float _fov = 60;
	float _size = 5;
	bool _isOrtographic = false;
	void updateFront();

public:

	Camera();

	// internal data
	vec3 position;
	vec3 direction;
	vec3 up;
	vec3 right;
	/*vec3 right;
	vec3 forward;*/

	void setDirection(vec3 position); // must call update transform!
	void setPitch(float pitch);
	void setYaw(float yaw);
	void setOrthogonal(float size = 5);
	void setPerspective(float fov = 60);
	bool isOrthgraphic();
	bool isPerspective();
	void updateTransform();
	const mat4 & getMatrix();
	Ray generateRay(float x, float y) const;

	void reset();

	int targetObject = -1;

};

#endif