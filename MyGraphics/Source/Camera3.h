#ifndef CAMERA_3_H
#define CAMERA_3_H

#include "Camera.h"
#include <Windows.h>

class Camera3 : public Camera
{
public:
	//Vector3 position;
	//Vector3 target;
	//Vector3 up;
	float worldScale;
	float angle;
	float CAMERA_SPEED;

	Vector3 oldpos;
	Vector3 oldtgt;
	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;

	Camera3();
	~Camera3();

	//Boundary checking
	void BoundsCheck(void);
	void Limiter(void);
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt);
	virtual void Reset();
};

#endif