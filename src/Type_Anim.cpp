#include "Type_Anim.h"
#include "ObjectManger.h"
#include <iostream>
using namespace std;

ObjectTypeArray<Type_Anim> Type_Anim::Array = ObjectTypeArray<Type_Anim>();

bool Type_Anim::LoadFromConfig(ConfigManger &manger) {LOGFUNC;
	Type_Object::LoadFromConfig(manger);
	Type_Object::Array.Push(RegName.c_str(), this);

	cout << "CubeCore: Type_Anim::LoadFromConfig - Loading " << RegName << " ..." << endl;
	auto object = manger.getObject(RegName);

	this->texture = TextureManger::GetInstance().TextureHashs[object["image"]];
	this->LoopCount = object["loop_count"];
	this->FrameStep = object["frame_step"];
	this->FrameLast = object["frame_last"];

	luabridge::LuaRef scale = object["scale"];
	int a = scale["x"], b = scale["y"];
	this->Scale.x = a/100.0f, this->Scale.y = b/100.0f;
	_scale = sf::Vector2f(Scale.x, Scale.y);

	luabridge::LuaRef offset = object["offset"];
	a = offset["x"], b = offset["y"];
	this->Offset.x = a, this->Offset.y = b;

	object["PTR"] = this;
	
	return true;
}

Abs_Anim &Type_Anim::createAnim(Type_Anim *Type) {LOGFUNC;
	return *(new Abs_Anim(Type));
}