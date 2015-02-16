#ifndef _UTILITY_H
#define _UTILITY_H

#include "Vertex.h"
#include "Mtx44.h"

Position operator*(const Mtx44& lhs, Position rhs);

#endif