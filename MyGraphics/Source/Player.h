#pragma once

#include "Camera3.h"
#include "Vector3.h"
#include "Application.h"

class CPlayer
{
private:
	Vector3 position;

public:
	CPlayer(void);
	~CPlayer(void);

	void Init();
	void Update(double dt);
	void Render();
	void Exit();

	Camera3 camera;
	bool modeCustomer;
	bool modeGuard;
	bool modeVillain;

};

