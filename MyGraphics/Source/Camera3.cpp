#include "Camera3.h"
#include "Application.h"
#include "Mtx44.h"

Camera3::Camera3()
{
	worldScale = 4004.f;
	angle = 0;
}

Camera3::~Camera3()
{
}

void Camera3::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();
}

void Camera3::Update(double dt)
{
	CAMERA_SPEED = 100.f;

	if(Application::IsKeyPressed(VK_LEFT))
	{
		Vector3 view = (target - position).Normalized();
		float yaw = (float)(CAMERA_SPEED * dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		view = rotation * view;
		up = rotation * up;
		target = view + position;
		
		//Vector3 view = (target - position).Normalized();
		//Vector3 right = view.Cross(up);
		//right.Normalize();
		//right.y = 0;
		//up = right.Cross(view).Normalized();

		//float yaw = (float)(CAMERA_SPEED * dt);
		//Mtx44 rotation;
		//rotation.SetToRotation(yaw, up.x, up.y, up.z);
		//view = rotation * view;
		//target = view + position;
	}
	if(Application::IsKeyPressed(VK_RIGHT))
	{
		Vector3 view = (target - position).Normalized();
		float yaw = (float)(-CAMERA_SPEED * dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		view = rotation * view;
		up = rotation * up;
		target = view + position;

		//Vector3 view = (target - position).Normalized();
		//Vector3 right = view.Cross(up);
		//right.Normalize();
		//right.y = 0;
		//up = right.Cross(view).Normalized();
		//float yaw = (float)(-CAMERA_SPEED * dt);
		//Mtx44 rotation;
		//rotation.SetToRotation(yaw, up.x, up.y, up.z);
		//view = rotation * view;
		//target = view + position;
	}
	if(Application::IsKeyPressed(VK_UP))
	{
		float pitch = 0.f;
		if(angle < 75.f)
		{
			angle += (float)(CAMERA_SPEED * dt);
			pitch = (float)(CAMERA_SPEED * dt);
		}
		else
		{
			pitch = 0;
		}
		
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		view = rotation * view;
		target = view + position;
	}
	if(Application::IsKeyPressed(VK_DOWN))
	{
		float pitch = 0.f;
		if(angle > -75.f)
		{
			angle += (float)(-CAMERA_SPEED * dt);
			pitch = (float)(-CAMERA_SPEED * dt);
		}
		else
		{
			pitch = 0;
		}
		
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		view = rotation * view;
		target = view + position;
	}
	
	if(Application::IsKeyPressed('A'))
	{
		oldpos = position;
		oldtgt = target;

		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		position -= right * CAMERA_SPEED * dt;
		target -= right * CAMERA_SPEED * dt;
		BoundsCheck();
	}
	if(Application::IsKeyPressed('D'))
	{
		oldpos = position;
		oldtgt = target;

		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		position += right * CAMERA_SPEED * dt;
		target += right * CAMERA_SPEED * dt;
		BoundsCheck();
	}
	if(Application::IsKeyPressed('W'))
	{
		oldpos = position;
		oldtgt = target;
		Vector3 view = (target - position).Normalized();
		view.y = 0;
		view.Normalize();
		position += view * CAMERA_SPEED * dt;
		target += view * CAMERA_SPEED * dt;
		BoundsCheck();
	}
	if(Application::IsKeyPressed('S'))
	{
		oldpos = position;
		oldtgt = target;
		Vector3 view = (target - position).Normalized();
		view.y = 0;
		view.Normalize();
		position -= view * CAMERA_SPEED * dt;
		target -= view * CAMERA_SPEED * dt;
		BoundsCheck();
	}
	//if(Application::IsKeyPressed('R'))
	//{
	//	Reset();
	//}
}

void Camera3::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}

void Camera3::BoundsCheck(void)
{
	//Fill in position here and check
}

//Collision
void Camera3::Limiter(void)
{
	position = oldpos;
	target = oldtgt;
	oldpos = position;
	oldtgt = target;
}