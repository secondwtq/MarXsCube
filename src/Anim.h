#ifndef CUBE_E_ANIM_H
#define CUBE_E_ANIM_H

#include "Common.h"
#include "Object.h"
#include "Type_Anim.h"
#include "SilconSprite.h"

#include "Config.h"

class Abs_Anim : public Abs_Object {

	public:
		Abs_Anim(Type_Anim *Type);

		unsigned int WhatAmI() { return RTTIType_Anim; }
		Type_Anim *AnimType = nullptr;
		CubeTimer AnimTimer = CubeTimer(1, -1);
		SilconSprite sprite;
		float scale = 1.f;

		Abs_Object *getObjectPtr() {LOGFUNC; return (Abs_Object *)this; }

		void Render();
		void Update();

		void StartPlay();

		void SpawnAtMapCoord(const CoordStruct &location);

		unsigned int GetCurrentFrame();

		CubeTimer &getAnimTimer() { return AnimTimer; }

		Type_Anim *getAnimType() { return AnimType; }

};

#endif