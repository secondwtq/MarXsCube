//
//  PhysicsGeneral.cpp
//  MarXsCube
//
//  Created by SeconDatke on 14-9-22.
//  Copyright (c) 2014年 MarXsCube Staff. All rights reserved.
//

#include "Common.h"
#include "Physics.h"
#include "PhysicsGeneral.h"

Float3D GlobalGravity = Float3D(0, 0, -100);

void PhysicsGeneral::init() {
	collConf = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collConf);
	overlapPairCache = new btDbvtBroadphase(); // btAxis3Sweep
	solver = new btSequentialImpulseConstraintSolver();
	
	dynaWorld = new btDiscreteDynamicsWorld(dispatcher, overlapPairCache, solver, collConf);
	dynaWorld->setGravity(btVector3(btScalar(GlobalGravity.x), btScalar(GlobalGravity.y), btScalar(GlobalGravity.z)));
	
	dynaWorld->setInternalTickCallback(cubeTickCallback);
	
//	btCollisionShape *ground = new btBoxShape(btVector3(btScalar(30 * 64), btScalar(30 * 64), btScalar(0.1)));
//	btTransform groundTrans;
//	groundTrans.setIdentity();
//	groundTrans.setOrigin(btVector3(30*64, 0, 0));
//	btDefaultMotionState *grMotionState = new btDefaultMotionState(groundTrans);
//	btRigidBody::btRigidBodyConstructionInfo grInfo(0., grMotionState, ground);
//	btRigidBody *grBody = new btRigidBody(grInfo);
//	grBody->setRestitution(btScalar(1));
//	grBody->setContactProcessingThreshold(btScalar(64));
//	
//	auto physicObj_Ground = new PhysicsObject(true);
//	physicObj_Ground->isCell = true;
//	grBody->setUserPointer(physicObj_Ground);
//	dynaWorld->addRigidBody(grBody);
}

void PhysicsGeneral::dispose() {
	delete dynaWorld;
	delete solver;
	delete overlapPairCache;
	delete dispatcher;
	delete collConf;
}