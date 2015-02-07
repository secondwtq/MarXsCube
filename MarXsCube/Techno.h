#ifndef CUBE_E_TECHNO_H
#define CUBE_E_TECHNO_H

#include "Common.h"
#include "Object.h"
#include "Type_Techno.h"
#include "SFML.h"

#include "Pathfinder.h"

#include <unordered_map>

class RenderElementsContainer;

class Abs_Techno : public Abs_Object {
	public:
		Type_Techno *TechnoType;
		static RenderLayerType::Type GetDefaultLayer() {LOGFUNC; return RenderLayerType::LAYER_OBJECT; }

		unsigned int WhatAmI() { return RTTIType_Techno; }

		RenderElementsContainer *elements;

		void SpawnAtMapCoord(const CoordStruct &location);

		Type_Techno *getTechnoType() {LOGFUNC; return TechnoType; }

		Abs_Techno(Type_Techno *Type);
		Abs_Techno(Type_Techno *Type, PhysicsObject *phy_object);

		void Update();
		void Render();

		~Abs_Techno();

		Pathfinding::PathFindingcache *getPathfindingcache() { return &pathfindingcache; }
		Pathfinding::PathFindingcache pathfindingcache;
};

#include "RenderElement.h"

#endif