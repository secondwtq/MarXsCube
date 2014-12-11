//
//  RayTest.h
//  MarXsCube
//
//  Created by SeconDatke on 14-12-11.
//  Copyright (c) 2014年 MarXsCube Staff. All rights reserved.
//

#ifndef __MarXsCube__RayTest__
#define __MarXsCube__RayTest__

#include "Common.h"
#include "PhysicsCommon.h"

namespace Physics {
	namespace RayTest {
		namespace _impl {
			
			class ClosestRayResultCallback_Custom : public btCollisionWorld::ClosestRayResultCallback {
			public:
				ClosestRayResultCallback_Custom(const btVector3& rayFromWorld,const btVector3& rayToWorld)
				: ClosestRayResultCallback(rayFromWorld, rayToWorld) {LOGFUNC; }
				bool needsCollision(btBroadphaseProxy* proxy0) const;
			};
			
			class ClosestRayResultCallback_ForCell : public ClosestRayResultCallback_Custom {
			public:
				ClosestRayResultCallback_ForCell(const btVector3& rayFromWorld,const btVector3& rayToWorld)
				: ClosestRayResultCallback_Custom(rayFromWorld, rayToWorld) {LOGFUNC; }
				bool needsCollision(btBroadphaseProxy* proxy0) const;
			};
			
			class ClosestRayResultCallback_AlwaysTrue : public btCollisionWorld::ClosestRayResultCallback {
			public:
				ClosestRayResultCallback_AlwaysTrue(const btVector3& rayFromWorld,const btVector3& rayToWorld)
				: ClosestRayResultCallback(rayFromWorld, rayToWorld) {LOGFUNC; }
				bool needsCollision(btBroadphaseProxy* proxy0) const {LOGFUNC; return (true); }
			};
			
			template <typename CallbackT>
			class RayTestClass {
			public:
				CallbackT callback;
				RayTestClass(const CoordStruct& start, const CoordStruct& end) : callback(btVector3(btScalar(start.x), btScalar(start.y), btScalar(start.z)),
																			btVector3(btScalar(end.x), btScalar(end.y), btScalar(end.z))) {LOGFUNC; }
				void perform() {LOGFUNC;
					Generic::PhysicsGeneral()->dynaWorld->rayTest(callback.m_rayFromWorld, callback.m_rayToWorld, callback); }
				
				PhysicsObject *getFirstObject() {LOGFUNC;
					return (PhysicsObject *)(callback.m_collisionObject->getUserPointer()); }
				
				bool hit() {LOGFUNC; return callback.hasHit(); }
				
				CoordStruct hit_point() {
					return CoordStruct(this->callback.m_hitPointWorld.x(), this->callback.m_hitPointWorld.y(), this->callback.m_hitPointWorld.z()); }
				
			};
		}
		
		using RayTestSingle = _impl::RayTestClass<_impl::ClosestRayResultCallback_Custom>;
		using RayTestSingleForCell = _impl::RayTestClass<_impl::ClosestRayResultCallback_ForCell>;

		RayTestSingle *createRayTestSingle(CoordStruct &start, CoordStruct &end);
		RayTestSingleForCell *createRayTestForCell(CoordStruct &start, CoordStruct &end);
	}
}


#endif /* defined(__MarXsCube__RayTest__) */
