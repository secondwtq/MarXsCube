#include "Object.h"

Abs_Object::Abs_Object(Type_Object *Type) : ObjectType(Type) {LOGFUNC;
	Physics->Type = &Type->PhysicsType;
	Layer = Abs_Object::GetDefaultLayer();
}

Abs_Object::Abs_Object(Type_Object *Type, PhysicsObject *phy_object) :
		Abs_Abstract(phy_object), ObjectType(Type) {LOGFUNC;
	if (this->Physics) Physics->Type = &Type->PhysicsType;
	Layer = Abs_Object::GetDefaultLayer();
}