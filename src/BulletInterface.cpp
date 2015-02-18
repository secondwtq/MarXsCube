//
//  BulletInterface.cpp
//  MarXsCube
//
//  Created by SeconDatke on 2/18/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "Config.h"
#include "Generic.h"
#include "BulletDebugDraw.h"
#include "BulletInterface.h"

void BulletDebug_DrawLine(const CoordStruct& pt1, const CoordStruct& pt2) {
	BulletDebugDrawer *drawer = reinterpret_cast<BulletDebugDrawer *>(Generic::PhysicsGeneral()->dynaWorld->getDebugDrawer());
	
	btVector3 from = btVector3(btScalar(pt1.x*PHY_SCALE), btScalar(pt1.y*PHY_SCALE), btScalar(pt1.z*PHY_SCALE));
	btVector3 to = btVector3(btScalar(pt2.x*PHY_SCALE), btScalar(pt2.y*PHY_SCALE), btScalar(pt2.z*PHY_SCALE));
	drawer->push_task([from, to] (BulletDebugDrawer * d) {
		d->drawLine(from, to, btVector3(1.0, 1.0, 1.0)); });
}

void LuaInterface::RegisterInterface_Bullet(LuaStatus& L) {
	luabridge::getGlobalNamespace(L).
	beginNamespace("Bullet").
		beginNamespace("DebugDrawer").
			addFunction("draw_line", BulletDebug_DrawLine).
		endNamespace().
	endNamespace();
}
