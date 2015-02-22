#ifndef CUBE_E_OBJECT_H
#define CUBE_E_OBJECT_H

#include "Common.h"
#include "Abstract.h"
#include "Pathfinder.h"

class Abs_Cell;
class Type_Object;

class Abs_Object : public Abs_Abstract {
	
	public:
		static RenderLayerType::Type GetDefaultLayer() { return RenderLayerType::LAYER_OBJECT; }

		unsigned int WhatAmI() { return RTTIType_Object; }

		CoordStruct GetCoord() const { return Location; }

		int GetHeight() { return Location.z; }

		virtual void SpawnAtMapCoord(const CoordStruct &location) { Location = location; }

		void setLocation(const CoordStruct &location) { Location = location; }

		void setMainRotation(int deg) {
			if (deg < 0) deg = 360 + deg;
			Direction.degree = deg;
		}

		int getMainRotation() { return Direction.degree; }

		Abs_Object(Type_Object *Type);
		Abs_Object(Type_Object *Type, PhysicsObject *phy_object);

		bool onGround() { return abs(GetHeight()) <= 5; }
	
		void setDestnation(const CoordStruct &destnation) { Destnation = destnation; }

		Type_Object *ObjectType = nullptr;
	
		CoordStruct Destnation = CoordStruct(0, 0, 0);
	
		ObjectDirection Direction;
		ObjectDirection *getDirection() {LOGFUNC; return &Direction; }

		Pathfinding::Repathcache *getRepathcache() { return &repathcache; }
		Pathfinding::Repathcache repathcache;

	public:
		CoordStruct Location = CoordStruct(0, 0, 0);
};

#include "Cell.h"
#include "Type_Object.h"

#endif