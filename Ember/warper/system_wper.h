#pragma once

#include "macro/macro.h"

#ifdef _WIN32
#include <Windows.h>
#endif

NAMESPACEBEGINE
inline void sleep(int mill)
{
	Sleep(mill);
}
NAMESPACEEND