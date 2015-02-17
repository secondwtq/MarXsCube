#ifndef CUBE_E_TYPE_ABSTRACT_H
#define CUBE_E_TYPE_ABSTRACT_H

#include <string>
#include "Common.h"
#include "Abstract.h"
#include "ObjectTypeArray.h"
#include "Physics.h"

class ConfigManger;

class Type_Abstract : public Abs_Abstract {
	public:
		static ObjectTypeArray<Type_Abstract> Array;

		unsigned int WhatAmI() {LOGFUNC; return RTTIType_AbstractType; }

		virtual bool LoadFromConfig(ConfigManger &manger);

		std::string RegName;
		bool EnablePhysics = false;

		int t = 0;

		PhysicsObjectType PhysicsType;

	public:

		Type_Abstract(const char *ID);

		CoordStruct GetCoord() const {LOGFUNC; return CoordStruct(-1, -1, -1); }
		int GetHeight() { return -1; }
		void Render() { };
		void Update() { };

		luabridge::LuaRef typeRef;
};

#include "Config.h"

#endif