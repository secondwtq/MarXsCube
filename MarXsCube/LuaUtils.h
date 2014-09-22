#ifndef CUBE_E_LUAUTIL_H
#define CUBE_E_LUAUTIL_H

#include "Common.h"
#include "Techno.h"

namespace LuaUtils {
	void initLuaUtils();

	void closeSession();

	CoordStruct GetCoordFromPoint(CubePoint pt);

	double GetTime();
	double GetTime_Clock();

	Abs_Techno *toTechno(Abs_Abstract *src);
}

#endif