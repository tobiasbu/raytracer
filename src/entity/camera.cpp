
#include "entity/camera.h"
#include "math/matrixtransform.h"
#include "math/maths.h"


Camera::Camera() {

	position.set(0, 0, -5);
	setDirection(vec3(0, 0, 0));
	up.set(0, -1, 0);

	_viewMatrix = mat4::identity;
	

	_yaw = -89;
	_pitch = 0;

	//updateFront();
	updateTransform();
}

void Camera::reset() {
	position.set(0, 0, -5);
	setDirection(vec3(0, 0, 0));
	up.set(0, -1, 0);

	_viewMatrix = mat4::identity;


	_yaw = -89;
	_pitch = 0;

	//updateFront();
	updateTransform();
}

void Camera::setDirection(vec3 target) {

	this->_target = target;
	direction = (position - target).normalized();
	
	// update pitch and yaw
	_pitch = atan2f(direction.y, direction.x) * RAD_TO_DEG;

}

void Camera::setPitch(float pitch) {

	_pitch = pitch;

	/*if (_pitch > 89.0f)
		_pitch = 89.0f;

	if (_pitch < -89.0f)
		_pitch = -89.0f;*/

	updateFront();

}

void Camera::setYaw(float yaw) 
{
	_yaw = yaw;//(yaw) * PI / 180.;

	updateFront();

}

void Camera::setOrthogonal(float size) 
{
	_size = size;
	_isOrtographic = true;
}

void Camera::setPerspective(float fov)
{
	_fov = fov * DEG_TO_RAD;
	_isOrtographic = false;
	
}

bool Camera::isOrthgraphic()
{
	return _isOrtographic;
}

bool Camera::isPerspective() 
{
	return !_isOrtographic;
}

Ray Camera::generateRay(float x, float y) const
{
	vec3 dir;

	if (!_isOrtographic) 
	{
		float halfAngle = _fov / 2.0f;
		float hsize = tanf(halfAngle);
		float d = -halfAngle/2.0f;
		float left = -hsize * 1.33f;
		float right = hsize * 1.33f;
		float top = hsize;
		float bottom = -hsize;
		float u = bottom + (top - bottom)*y;
		float v = left + (right - left)*x;

		dir = (d*direction) + (u*up) + (v*this->right);

		
	}


	return Ray(position, dir);
}

void Camera::updateFront() {

	float pitr = _pitch * DEG_TO_RAD;
	float yawr = _yaw * DEG_TO_RAD;
	
	direction.x = cos(yawr) * cos(pitr);
	direction.y = sin(pitr);
	direction.z = sin(yawr) * cos(pitr);
	direction.normalized();

}


void  Camera::updateTransform() {

	_viewMatrix = MatrixTransform::lookAt(position, position + direction, up);
	
	right = vec3::cross(direction, up).normalized();

	//mat4 R = MatrixTransform::rotate_y_deg(mat4::identity, 0);
	//_viewMatrix = MatrixTransform::translate(mat4::identity, position);
	//_viewMatrix = R * _viewMatrix;
	//_viewMatrix = MatrixTransform::lookAt(position, vec3::zero, up);
	/*_viewMatrix = mat4(0.945519, 0, -0.325569, 0,
					-0.179534, 0.834209, -0.521403, 0,
					0.271593, 0.551447, 0.78876, 0,
					4.208271, 8.374532, 17.932925, 1);*/

	// this the same as MatrixTransform::lookAt
	//MatrixTransform::setPosition(_viewMatrix, position);
	//MatrixTransform::setDirection(_viewMatrix, direction);
	//MatrixTransform::setUp(_viewMatrix, direction, up);

	//_viewMatrix = MatrixTransform::lookAt2(position, vec3(0,0,0), up); 
	

	/*MatrixTransform::setPosition(_viewMatrix, position);
	MatrixTransform::setDirection(_viewMatrix, direction);
	MatrixTransform::setUp(_viewMatrix, position+direction, up);*/

	//viewMatrix.m[15] = 1;
//MatrixTransform::lookAt(position, direction, up);

}

const mat4 & Camera::getMatrix() {

	return _viewMatrix;

}