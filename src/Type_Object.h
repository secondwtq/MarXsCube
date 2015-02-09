#ifndef CUBE_E_TYPE_OBJECT_H
#define CUBE_E_TYPE_OBJECT_H

#include "Common.h"
#include "Type_Abstract.h"

class Type_Object : public Type_Abstract {

	public:
		static ObjectTypeArray<Type_Object> Array;

		Type_Object(const char *ID) : Type_Abstract(ID) {LOGFUNC; }

		unsigned int WhatAmI() { return RTTIType_ObjectType; }
		bool LoadFromConfig(ConfigManger &manger);

};

#endif