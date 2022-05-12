#include "pch.h"
#include "Camera.h"
#include "utils.h"
#include "glutils.h"
#include "matrix4x4.h"



// Constructor with default position at z = -3
Camera::Camera()
{	
	_view_from = Vector3(0.0f, 0.0f, 0.0f);
	_view_at = Vector3(1.0f, 0.0f, 0.0f);	
	_camera_position = _view_from;
	_Yaw = atan2(_view_at.z, _view_at.x) * 180 / (float)M_PI;
	_Pitch = asin(_view_at.y) * 180 / (float)M_PI;;
	_MouseSensitivity = 0.1f;
	_MovementSpeed = 10.5f;
	_Zoom = 45.0f;
	Camera::update();
}
// Constructor with different default position
Camera::Camera(Vector3 view_from, Vector3 view_at, float Yaw, float Pitch, float MouseSensitivity, float MovementSpeed, float Zoom)
{
	_view_from = view_from;
	_view_at = view_at;
	_camera_position = _view_from;
	_Yaw = Yaw;
	_Pitch = Pitch; 
	_MouseSensitivity = MouseSensitivity;
	_MovementSpeed = MovementSpeed;
	_Zoom = Zoom;
	Camera::update();
}

void Camera::BuildMVPMatrix()
{
	
}

void Camera::Translate(Matrix4x4 &model, double move_x, double move_y, double move_z) {
	
	model.set(0, 3, move_x);
	model.set(1, 3, move_y);
	model.set(2, 3, move_z);
}
void Camera::Scale(Matrix4x4 &model, double scale_x, double scale_y, double scale_z) {
	
	model.set(0, 0, scale_x);
	model.set(1, 1, scale_y);
	model.set(2, 2, scale_z);
}

void Camera::Scale_uniform(Matrix4x4& model, double scale) {

	model.set(3, 3, scale);
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
	float velocity = _MovementSpeed * deltaTime;
	if (direction == FORWARD)
		_camera_position += _camera_front * velocity;
	if (direction == BACKWARD)
		_camera_position -= _camera_front * velocity;
	if (direction == LEFT)
		_camera_position -= _camera_right * velocity;
	if (direction == RIGHT)
		_camera_position += _camera_right * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset)
{
	GLboolean constrainPitch = true;
	xoffset *= _MouseSensitivity;
	yoffset *= _MouseSensitivity;

	_Yaw += xoffset;
	_Pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (_Pitch > 89.0f)
			_Pitch = 89.0f;
		if (_Pitch < -89.0f)
			_Pitch = -89.0f;
	}
	// update Front, Right and Up Vectors using the updated Euler angles
	update();	
}

Matrix4x4 Camera::GetViewMatrix() {
	Matrix4x4 view = BuildViewMatrix(_camera_position, _camera_position + _camera_front, _camera_up);
	return view;
}

Matrix4x4 Camera::BuildViewMatrix(Vector3 view_from, Vector3 look_at, Vector3 world_up)
{
	Vector3 z_e = view_from - look_at;
	z_e.Normalize();
	Vector3 up = world_up;
	up.Normalize();
	Vector3 x_e = up.CrossProduct(z_e);
	x_e.Normalize();
	Vector3 y_e = z_e.CrossProduct(x_e);
	y_e.Normalize();
	Matrix4x4 view = Matrix4x4(x_e, y_e, z_e, view_from);
	view.EuclideanInverse();

	return view;

}

void Camera::update()
{
	// calculate the new Front vector
	Vector3 front;	
	//front.x = cos()
	front.x = cos(My_Math::toRadians(_Yaw)) * cos(My_Math::toRadians(_Pitch));
	front.y = sin(My_Math::toRadians(_Pitch));
	front.z = sin(My_Math::toRadians(_Yaw)) * cos(My_Math::toRadians(_Pitch));
	front.Normalize();
	_camera_front = front;
	// also re-calculate the Right and Up vector
	_camera_right = _camera_front.CrossProduct(_world_up);
	_camera_right.Normalize(); // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	_camera_up = _camera_right.CrossProduct(_camera_front);
	_camera_up.Normalize();
		
}

Matrix4x4 Camera::BuildProjectionMatrix(float aspect, int fov_vertical, float near_plane, float far_plane)
{
	// near a far clipping
	float n = near_plane;
	float f = far_plane;
	float a = (n + f) / (n - f);
	float b = (2.0f * n * f) / (n - f);
			
	float h = tan(fov_vertical * 0.5f);
	float w = aspect * h;
	

	
	//float fov_y = 2 * atan(pow(aspect, -1) * tan(fov_x / 2)); // vertical FOV
	
	
	Matrix4x4 P = Matrix4x4();
	P.set(0, 0, 1.0f / w);
	P.set(1, 1, -1.0f / h);
	P.set(2, 2, a);
	P.set(2, 3, b);
	P.set(3, 2, -1.0f);
	P.set(3, 3, 0.0f);
	return P;
	/*
	

	Matrix4x4 N = Matrix4x4();
	N.set(0, 0, 2.0f / w);
	N.set(1, 1, 2.0f / h);
	Matrix4x4 D = Matrix4x4();
	
	D.set(0, 0, n);
	D.set(1, 1, n);
	D.set(2, 2, a);
	D.set(2, 3, b);
	D.set(3, 2, -1.0f);
	D.set(3, 3, 0.0f);
	
	return N*D;
	*/
}



