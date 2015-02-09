//
//  PhysicsGeneral.h
//  MarXsCube
//
//  Created by SeconDatke on 14-9-22.
//  Copyright (c) 2014年 MarXsCube Staff. All rights reserved.
//

#ifndef MarXsCube_PhysicsGeneral_h
#define MarXsCube_PhysicsGeneral_h

#include <Bullet/btBulletDynamicsCommon.h>

class CPhysicsGeneral {
	public:
		btDefaultCollisionConfiguration *collConf;
		btCollisionDispatcher *dispatcher;
		btBroadphaseInterface *overlapPairCache;
		btSequentialImpulseConstraintSolver *solver;
		btDiscreteDynamicsWorld *dynaWorld;
		
	public:
		void init();
		void dispose();
		
	private:
		void init_debugdrawer();
};

void cubeTickCallback(btDynamicsWorld *world, btScalar timeStep);

extern Float3D GlobalGravity;

#endif
