#pragma once
#include <math.h>
#include <macro/macro.h>
#define STREAMUNITSIZE 512

NAMESPACEBEGINE
inline bool Equred(double f, double r, double util = 0.00000000001)
{
	return f / util == r / util;
}
NAMESPACEEND