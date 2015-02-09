#include "Common.h"
#include "Type_Techno.h"
#include "Techno.h"

ObjectTypeArray<Type_Techno> Type_Techno::Array = ObjectTypeArray<Type_Techno>();

bool Type_Techno::LoadFromConfig(ConfigManger &manger) {LOGFUNC;
	bool ret = Type_Object::LoadFromConfig(manger);
	std::cout << "CubeCore: Type_Techno::LoadFromConfig - Loading " << RegName << " ..." << std::endl;
	Type_Techno::Array.Push(RegName.c_str(), this);
	auto object = manger.getObject(RegName);
	EventManger::GetInstance().GetEvent(EventManger::Events::TECHNOTYPE_LOAD)(this, object);
	object["PTR"] = this;
	return (ret | true);
}

Abs_Techno *Type_Techno::createTechno(Type_Techno *Type) {
	return new Abs_Techno(Type); }

Abs_Techno *Type_Techno::createTechno_nophy(Type_Techno *Type) {
	return new Abs_Techno(Type, nullptr); }