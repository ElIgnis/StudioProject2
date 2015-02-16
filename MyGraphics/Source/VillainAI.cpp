#include "VillainAI.h"


CVillainAI::CVillainAI(void)
{
}


CVillainAI::~CVillainAI(void)
{
}

void CVillainAI::setPositionX(int x)
{
	position.x = x;
}
void CVillainAI::setPositionZ(int z)
{
	position.z = z;
}

Vector3 CVillainAI::getPosition(void)
{
	return position;
}
