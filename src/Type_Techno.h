#ifndef CUBE_E_TYPE_TECHNO_H
#define CUBE_E_TYPE_TECHNO_H

#include "Common.h"
#include "Type_Object.h"

class Abs_Techno;

class Type_Techno : public Type_Object {
	public:
		static ObjectTypeArray<Type_Techno> Array;

		unsigned int WhatAmI() { return RTTIType_TechnoType; }
		
		Type_Techno(const char *ID) : Type_Object(ID) {LOGFUNC; }
		bool LoadFromConfig(ConfigManger &manger);

		static Abs_Techno *createTechno(Type_Techno *Type);
		static Abs_Techno *createTechno_nophy(Type_Techno *Type);
};

#include "Techno.h"

#endif