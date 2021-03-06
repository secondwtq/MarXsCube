#ifndef CUBE_E_ABSTRACT_H
#define CUBE_E_ABSTRACT_H

#include "Common.h"
#include "HafniumCommon.h"
#include "PhysicsCommon.h"
#include "ObjectArray.h"

class PhysicsObject;

class Abs_Abstract {
	public:

		static ObjectArray<Abs_Abstract *> Array;

		virtual unsigned int WhatAmI() { return RTTIType_Abstract; }

		static RenderLayerType::Type GetDefaultLayer() { return RenderLayerType::LAYER_NONE; }

		virtual CoordStruct GetCoord() const = 0;

		virtual void Render() = 0;
		virtual void Update() = 0;

		virtual int GetHeight() = 0;

		RenderLayerType::Type Layer = GetDefaultLayer();

		void setLayer(int Layer);
		void print_this() { printf("Type: %d, ID: %u.\n", this->WhatAmI(), this->RTTIID); }

		virtual int getMainRotation() { return 0; }
		virtual void setLocation(const CoordStruct &location) { }
		virtual void setMainRotation(int deg) { }

		virtual bool onGround() { return false; }

		unsigned int RTTIID = 0;

		Abs_Abstract();
		Abs_Abstract(PhysicsObject *phy_object);
		virtual ~Abs_Abstract();

	public:

		bool useCollSphere = true;
		int rCollSphere = 64;
		btCollisionShape *collSphere = nullptr;

		bool EnablePhysics = false;
		PhysicsObject *Physics;

		luabridge::LuaRef ExtTable;
};

#include "Physics.h"

#endif