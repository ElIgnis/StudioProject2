#include "GuardAI.h"


CGuardAI::CGuardAI(void)
{
}


CGuardAI::~CGuardAI(void)
{
}

void CGuardAI::setPositionX(int x)
{
	position.x = x;
}
void CGuardAI::setPositionZ(int z)
{
	position.z = z;
}

Vector3 CGuardAI::getPosition(void)
{
	return position;
}