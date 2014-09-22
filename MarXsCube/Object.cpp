#include "Object.h"

Abs_Object::Abs_Object(Type_Object *Type) : ObjectType(Type) {LOGFUNC;
	Physics->Type = &Type->PhysicsType;
	Layer = Abs_Object::GetDefaultLayer();
}

