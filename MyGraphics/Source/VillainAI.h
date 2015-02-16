#pragma once

#include "Vector3.h"

class CVillainAI
{
private:
	Vector3 position;
public:
	CVillainAI(void);
	~CVillainAI(void);

	void setPositionX(int x);
	void setPositionZ(int z);
	Vector3 getPosition(void);
};

