//
//  PhysicsGeneral.cpp
//  MarXsCube
//
//  Created by SeconDatke on 14-9-22.
//  Copyright (c) 2014年 MarXsCube Staff. All rights reserved.
//

#include "Common.h"
#include "Physics.h"

#include "BulletDebugDraw.h"

#include "ATVBCube.h"

#include "PhysicsGeneral.h"

using namespace ATVBCube::Helper;

Float3D GlobalGravity = Float3D(0, 0, -9.8);

void PhysicsGeneral::init() {
	ATVBCube::load<S::BulletGeneralSetting>();
	GlobalGravity = ATVBCube::setting<S::BulletGeneralSetting>().gravity;
	
	collConf = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collConf);
//	overlapPairCache = new btAxisSweep3(btVector3(-1000,-1280,-200), btVector3(2250,1280,200)); // btDbvtBroadphase(); // btAxis3Sweep
	overlapPairCache = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();
	
	dynaWorld = new btDiscreteDynamicsWorld(dispatcher, overlapPairCache, solver, collConf);
	dynaWorld->setGravity(btVector3(btScalar(GlobalGravity.x), btScalar(GlobalGravity.y), btScalar(GlobalGravity.z)));
	
	dynaWorld->setInternalTickCallback(cubeTickCallback);
	
	
	BulletDebugDrawer *debug_drawer = new BulletDebugDrawer();
	dynaWorld->setDebugDrawer(debug_drawer);
	
	auto DEBUGDRAW_GENERAL = btIDebugDraw::DBG_DrawWireframe;
	auto DEBUGDRAW_CONSTRAINTS = btIDebugDraw::DBG_DrawConstraints | btIDebugDraw::DBG_DrawConstraintLimits;
	
	debug_drawer->setDebugMode(DEBUGDRAW_GENERAL | DEBUGDRAW_CONSTRAINTS);
	
	ATVBCube::load<S::BulletDebugSetting>();
	debug_drawer->set_draw_scale(ATVBCube::setting<S::BulletDebugSetting>().scale);
	
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