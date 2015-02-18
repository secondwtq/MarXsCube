#ifndef CUBE_E_TYPE_ANIM_H
#define CUBE_E_TYPE_ANIM_H

#include "Common.h"
#include "Type_Object.h"
#include "SFML.h"

class Abs_Anim;

class Type_Anim : public Type_Object {

	public:
		static ObjectTypeArray<Type_Anim> Array;

		unsigned int WhatAmI() { return RTTIType_AnimType; }
		bool LoadFromConfig(ConfigManger &manger);

		TextureAtlas *texture = nullptr;
		int LoopCount = 1;
		int FrameStep = 1;
		int FrameLast = 1;

		Vector2DT<float> Scale = Vector2DT<float>(1., 1.);
		CoordStruct Offset = { 0, 0, 0 };
		sf::Vector2f _scale = sf::Vector2f(0, 0);

		Type_Anim(const char *ID) : Type_Object(ID) {LOGFUNC; };

		static Abs_Anim &createAnim(Type_Anim *Type);
};

#include "Anim.h"

#endif