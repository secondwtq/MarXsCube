//
//  TilerBullet.cpp
//  MarXsCube
//
//  Created by SeconDatke on 1/27/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "PhysicsCommon.h"
#include "Physics.h"

#include "ModelLoader_obj.h"
#include "TilerBullet.h"
#include "Generic.h"

extern objfile obj_test;

void transfer_bullet_shape(btTriangleMesh& dest, const objfile &src) {
	for (std::size_t i = 0; i < src.raw_faces.size(); i++) {
		glm::i32vec3 face = src.raw_faces[i];
		glm::vec3 gv1 = src.raw_verts[face[0]], gv2 = src.raw_verts[face[1]], gv3 = src.raw_verts[face[2]];
		btVector3 bv1 { gv1.y, gv1.x, gv1.z }, bv2 { gv2.y, gv2.x, gv2.z }, bv3 { gv3.y, gv3.x, gv3.z };
		dest.addTriangle(bv1, bv2, bv3);
	}
}

void init_terrain_physhape() {
	btTriangleMesh *mesh = new btTriangleMesh();
	transfer_bullet_shape(*mesh, obj_test);
	PhysicsShapeTypeMeshStatic *mesh_shape = new PhysicsShapeTypeMeshStatic();
	mesh_shape->set_mesh(mesh);
	
	btCollisionShape *ground_shape = mesh_shape->createShape();
	
	btTransform groundTrans;
	groundTrans.setIdentity();
	groundTrans.setOrigin(btVector3(0, 0, 0));
	btDefaultMotionState *grMotionState = new btDefaultMotionState(groundTrans);
	btRigidBody::btRigidBodyConstructionInfo grInfo(0., grMotionState, ground_shape);
	btRigidBody *grBody = new btRigidBody(grInfo);
	grBody->setRestitution(btScalar(1));
	grBody->setContactProcessingThreshold(btScalar(64));

	auto ground_phy = new PhysicsObject(true);
	ground_phy->isCell = true;
	grBody->setUserPointer(ground_phy);
	Generic::PhysicsGeneral()->dynaWorld->addRigidBody(grBody);
	
//	PhysicsObjectType *ground_type = new PhysicsObjectType();
//	ground_type->EnablePhysics = true;
//	ground_type->ShapeType = mesh_shape;
//	
//	auto ground_phy = new PhysicsObject(true);
//	ground_phy->Type = ground_type;
//	ground_phy->isCell = true;
//	ground_phy->SpawnAt({ 640, 0, 0 });
}
