#pragma once

#include "Vector3.h"

class CGuardAI
{
private:
	Vector3 position;
public:
	CGuardAI(void);
	~CGuardAI(void);

	void setPositionX(int x);
	void setPositionZ(int z);
	Vector3 getPosition(void);
};

