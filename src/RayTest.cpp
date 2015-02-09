//
//  RayTest.cpp
//  MarXsCube
//
//  Created by SeconDatke on 14-12-11.
//  Copyright (c) 2014年 MarXsCube Staff. All rights reserved.
//

#include "Common.h"
#include "PhysicsCommon.h"
#include "Physics.h"

#include "RayTest.h"

Physics::RayTest::RayTestSingle *Physics::RayTest::createRayTestSingle(CoordStruct &start, CoordStruct &end) {
	return new Physics::RayTest::RayTestSingle(start, end); }
Physics::RayTest::RayTestSingleForCell *Physics::RayTest::createRayTestForCell(CoordStruct &start, CoordStruct &end) {
	return new Physics::RayTest::RayTestSingleForCell(start, end); }

bool Physics::RayTest::_impl::ClosestRayResultCallback_Custom::needsCollision(btBroadphaseProxy* proxy0) const {LOGFUNC;
	PhysicsObject *ptr = (PhysicsObject *)(((btCollisionObject *)proxy0->m_clientObject)->getUserPointer());
	return (ptr && (!ptr->isEmpty));
}

bool Physics::RayTest::_impl::ClosestRayResultCallback_ForCell::needsCollision(btBroadphaseProxy* proxy0) const {LOGFUNC;
	PhysicsObject *ptr = (PhysicsObject *)(((btCollisionObject *)proxy0->m_clientObject)->getUserPointer());
	return !(ptr->isEmpty && !ptr->isCell);
}