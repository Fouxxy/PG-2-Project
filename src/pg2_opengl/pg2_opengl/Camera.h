#include "pch.h"
#include "utils.h"
#include "glutils.h"
#include "matrix4x4.h"
#include "My_Math.h"
#pragma once


class Camera
{
public:
	enum Camera_Movement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};
	float _MovementSpeed;
	float _MouseSensitivity;
	float _Zoom;
	float _Yaw;
	float _Pitch;

	Vector3 _view_from;
	Vector3 _view_at;
	Vector3 _world_up = Vector3(0.0f, 1.0f, 0.0f);  // y axis is up
	Vector3 _camera_position = Vector3(0.0f, 0.0f, 1.0f);
	Vector3 _camera_front = Vector3(0.0f, 0.0f, -1.0f);
	Vector3 _camera_up = Vector3(0.0f, 1.0f, 0.0f);
	Vector3 _camera_right;

	
	Camera();
	Camera(Vector3 view_from, Vector3 view_at, float Yaw, float Pitch, float MouseSensitivity, float MovementSpeed, float Zoom);
	void BuildMVPMatrix();
		
	Matrix4x4 BuildViewMatrix(Vector3 view_from, Vector3 look_at, Vector3 worldUp);
	Matrix4x4 BuildProjectionMatrix(float aspect, int fov, float near_plane, float far_plane);
	
	static void Translate(Matrix4x4& model, double move_x, double move_y, double move_z);
	static void Scale(Matrix4x4& model, double scale_x, double scale_y, double scale_z);
	static void Scale_uniform(Matrix4x4& model, double scale);
	void ProcessKeyboard(Camera_Movement direction, float deltaTime);

	void ProcessMouseMovement(float xoffset, float yoffset);

	Matrix4x4 GetViewMatrix();
	


private:
	void update();
	
	

};

	