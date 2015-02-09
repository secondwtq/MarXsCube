#include "Type_Abstract.h"
using namespace std;

ObjectTypeArray<Type_Abstract> Type_Abstract::Array = ObjectTypeArray<Type_Abstract>();

Type_Abstract::Type_Abstract(const char *ID) :
	Abs_Abstract(nullptr), RegName(ID), typeRef(((EventManger::GetInstance())._state->State)) {LOGFUNC;

}

bool Type_Abstract::LoadFromConfig(ConfigManger &manger) {LOGFUNC;
	std::cout << "CubeCore: Type_Abstract::LoadFromConfig - Loading " << RegName << " ..." << std::endl;
	auto object = manger.getObject(RegName);
	object["UID"] = this->RTTIID;
	object["PTR"] = this;
	// std::cout << "CubeCore: Type_Abstract::LoadFromConfig - Loading phys" << RegName << " ..." << std::endl;
	PhysicsType.LoadFromConfig(manger, object);
	typeRef = object;
	// std::cout << "CubeCore: Type_Abstract::LoadFromConfig - Loading compl" << RegName << " ..." << std::endl;
	return true;
}

void setT(Type_Abstract *t) {LOGFUNC; printf("setting t\n"); t->t = 10; }