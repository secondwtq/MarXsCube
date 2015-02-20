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
				: ClosestRayResultCallback(rayFromWorld, rayToWorld) { }
				bool needsCollision(btBroadphaseProxy* proxy0) const;
			};

			class ClosestRayResultCallback_ForCell : public ClosestRayResultCallback_Custom {
			public:
				ClosestRayResultCallback_ForCell(const btVector3& rayFromWorld,const btVector3& rayToWorld)
				: ClosestRayResultCallback_Custom(rayFromWorld, rayToWorld) { }
				bool needsCollision(btBroadphaseProxy* proxy0) const;
			};

			class ClosestRayResultCallback_TechnoOnly : public ClosestRayResultCallback_Custom {
			public:
				ClosestRayResultCallback_TechnoOnly(const btVector3& rayFromWorld,const btVector3& rayToWorld)
				: ClosestRayResultCallback_Custom(rayFromWorld, rayToWorld) { }
				bool needsCollision(btBroadphaseProxy* proxy0) const;
				
				Abs_Abstract *except = nullptr;
			};

			class ClosestRayResultCallback_AlwaysTrue : public btCollisionWorld::ClosestRayResultCallback {
			public:
				ClosestRayResultCallback_AlwaysTrue(const btVector3& rayFromWorld,const btVector3& rayToWorld)
				: ClosestRayResultCallback(rayFromWorld, rayToWorld) { }
				bool needsCollision(btBroadphaseProxy* proxy0) const { return true; }
			};

			template <typename CallbackT>
			class RayTestClass {
			public:
				CallbackT *callback = nullptr;
				RayTestClass(const CoordStruct& start, const CoordStruct& end) : callback(new CallbackT(btVector3(btScalar(start.x*PHY_SCALE), btScalar(start.y*PHY_SCALE), btScalar(start.z*PHY_SCALE)),
																			btVector3(btScalar(end.x*PHY_SCALE), btScalar(end.y*PHY_SCALE), btScalar(end.z*PHY_SCALE)))) { }
				
				virtual ~RayTestClass() { delete this->callback; }
				
				void perform() {LOGFUNC;
					Generic::PhysicsGeneral()->dynaWorld->rayTest(callback->m_rayFromWorld, callback->m_rayToWorld, *callback); }

				PhysicsObject *getFirstObject() {LOGFUNC;
					return (PhysicsObject *)(callback->m_collisionObject->getUserPointer()); }

				bool hit() { return callback->hasHit(); }

				CoordStruct hit_point() {
					return CoordStruct(this->callback->m_hitPointWorld.x()/PHY_SCALE, this->callback->m_hitPointWorld.y()/PHY_SCALE, this->callback->m_hitPointWorld.z()/PHY_SCALE); }

			};
			
			using RayTestSingleTechnoOnly = RayTestClass<_impl::ClosestRayResultCallback_TechnoOnly>;
		
		}
		
		class RayTestSingleTechnoOnly : public _impl::RayTestSingleTechnoOnly {
		public:
			
			RayTestSingleTechnoOnly(const CoordStruct& start, const CoordStruct& end) : _impl::RayTestClass<_impl::ClosestRayResultCallback_TechnoOnly>(start, end) { }
			
			void set_except(Abs_Abstract *src) {
				static_cast<_impl::ClosestRayResultCallback_TechnoOnly *>(this->callback)->except = src; }
			
		};

		using RayTestSingle = _impl::RayTestClass<_impl::ClosestRayResultCallback_Custom>;
		using RayTestSingleForCell = _impl::RayTestClass<_impl::ClosestRayResultCallback_ForCell>;
//		using RayTestSingleTechnoOnly = _impl::RayTestClass<_impl::ClosestRayResultCallback_TechnoOnly>;

		RayTestSingle *createRayTestSingle(CoordStruct &start, CoordStruct &end);
		RayTestSingleForCell *createRayTestForCell(CoordStruct &start, CoordStruct &end);
		RayTestSingleTechnoOnly *createRayTestTechnoOnly(CoordStruct &start, CoordStruct &end);
	}
}


#endif /* defined(__MarXsCube__RayTest__) */
