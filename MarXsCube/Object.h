#ifndef CUBE_E_OBJECT_H
#define CUBE_E_OBJECT_H

#include "Common.h"
#include "Abstract.h"
#include "Pathfinder.h"

class Abs_Cell;
class Type_Object;

class Abs_Object : public Abs_Abstract {
	public:
		static RenderLayerType::Type GetDefaultLayer() {LOGFUNC; return RenderLayerType::LAYER_OBJECT; }

		unsigned int WhatAmI() { return RTTIType_Object; }

		CoordStruct GetCoord() const {LOGFUNC; return Location; }

		int GetHeight() { return Location.z; }

		virtual void SpawnAtMapCoord(const CoordStruct &location) {LOGFUNC; Location = location; }

		Type_Object *ObjectType = nullptr;

		void setLocation(const CoordStruct &location) {LOGFUNC; Location = location; }

		void setMainRotation(int deg) {LOGFUNC;
			if (deg < 0) deg = 360 + deg;
			Direction.degree = deg;
		}

		int getMainRotation() { return Direction.degree; }

		Abs_Object(Type_Object *Type);

		bool onGround() { return abs(GetHeight()) <= 5; }

		CoordStruct Destnation = CoordStruct(0, 0, 0);
		void setDestnation(const CoordStruct &destnation) { Destnation = destnation; }
		
		ObjectDirection Direction;
		ObjectDirection *getDirection() {LOGFUNC; return &Direction; }

		Pathfinding::Repathcache *getRepathcache() { return &repathcache; }
		Pathfinding::Repathcache repathcache;

		// bool enablePhysicsOccp = false;
		// CoordStruct occpSize
		// std::vector<Abs_Cell *> vec_phyOccCells;
	private:
		CoordStruct Location = CoordStruct(0, 0, 0);
};

#include "Cell.h"
#include "Type_Object.h"

#endif