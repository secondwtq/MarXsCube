#ifndef CUBE_E_ABSTRACT_H
#define CUBE_E_ABSTRACT_H

#include "Common.h"
#include "ObjectArray.h"
#include <LuaJit_Bridge.h>
#include "PhysicsCommon.h"

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

		virtual int getMainRotation() { return 0; }
		virtual void setLocation(const CoordStruct &location) { }
		virtual void setMainRotation(int deg) { }

		virtual bool onGround() { return false; }

		unsigned int RTTIID = 0;

		Abs_Abstract();
		virtual ~Abs_Abstract();

	public:
		bool temp_ZOffset = false;
	
		bool useCollSphere = true;
		int rCollSphere = 64;
		btCollisionShape *collSphere = nullptr;

		bool EnablePhysics = false;
		PhysicsObject *Physics;

		luabridge::LuaRef ExtTable;
};

#include "Physics.h"

#endif