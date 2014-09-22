#ifndef CUBE_E_ANIM_H
#define CUBE_E_ANIM_H

#include "Common.h"
#include "Object.h"
#include "Type_Anim.h"
#include "SFML.h"
using namespace sf;

#include "Config.h"

class Abs_Anim : public Abs_Object {

	public:
		Abs_Anim(Type_Anim *Type);

		unsigned int WhatAmI() { return RTTIType_Anim; }
		Type_Anim *AnimType = nullptr;
		CubeTimer AnimTimer = CubeTimer(1, -1);
		Sprite renderSprite;

		Abs_Object *getObjectPtr() {LOGFUNC; return (Abs_Object *)this; }

		void Render();
		void Update();

		void StartPlay();

		void SpawnAtMapCoord(const CoordStruct &location);

		unsigned int GetCurrentFrame();

		CubeTimer &getAnimTimer() {LOGFUNC; return AnimTimer; }

		Type_Anim *getAnimType() {LOGFUNC; return AnimType; }

};

#endif