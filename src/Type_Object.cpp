#include "Type_Object.h"

ObjectTypeArray<Type_Object> Type_Object::Array = ObjectTypeArray<Type_Object>();

bool Type_Object::LoadFromConfig(ConfigManger &manger) {LOGFUNC;
	bool ret = Type_Abstract::LoadFromConfig(manger);
	std::cout << "CubeCore: Type_Object::LoadFromConfig - Loading " << RegName << " ..." << std::endl;
	Type_Object::Array.Push(RegName.c_str(), this);
	return (ret | true);
}