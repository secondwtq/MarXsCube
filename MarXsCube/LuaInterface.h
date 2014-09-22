#ifndef CUBE_E_LUAINTERFACE_H
#define CUBE_E_LUAINTERFACE_H

#include "Config.h"

namespace LuaInterface {

	void RegisterInterface(LuaStatus &L);

	void RegisterInterface_Timer(LuaStatus &L);

	void RegisterInterface_Abstract(LuaStatus &L);

	void RegisterInterface_Cell(LuaStatus &L);

	void RegisterInterface_Object(LuaStatus &L);

	void RegisterInterface_Anim(LuaStatus &L);

	void RegisterInterface_Techno(LuaStatus &L);

	void RegisterInterface_Map(LuaStatus &L);

	void RegisterInterface_Util(LuaStatus &L);

	void RegisterInterface_Physics(LuaStatus &L);

}

#endif