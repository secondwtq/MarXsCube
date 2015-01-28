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

void transfer_bullet_hull(btAlignedObjectArray<btVector3>& dest, const objfile &src) {
	for (std::size_t i = 0; i < src.raw_verts.size(); i++) {
		glm::vec3 vert = src.raw_verts[i];
		dest.push_back({ vert.y, vert.x, vert.z });
	}
}

#include "BulletCollision/CollisionDispatch/btInternalEdgeUtility.h"

static bool CustomMaterialCombinerCallback(btManifoldPoint& cp,	const btCollisionObjectWrapper* colObj0Wrap,int partId0,int index0,const btCollisionObjectWrapper* colObj1Wrap,int partId1,int index1)
{
	btAdjustInternalEdgeContacts(cp,colObj1Wrap,colObj0Wrap, partId1,index1, BT_TRIANGLE_CONVEX_DOUBLE_SIDED+BT_TRIANGLE_CONCAVE_DOUBLE_SIDED);
	return true;
}

extern ContactAddedCallback gContactAddedCallback;

#include "SFML.h"
#include "BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"

sf::Color sfImageGetPixelFloat(const sf::Image& image, float x, float y) {
	auto size = image.getSize();
	return image.getPixel(x*size.x, y*size.y);
}

void init_terrain_physhape() {
	sf::Texture *height_texture = &(TextureManger::GetInstance().TextureHashs["HEIGHTFIELD"]->texture);
	sf::Image heightimage = height_texture->copyToImage();
	
	float *heightfield_data = new float[30*30];
	for (std::size_t i = 0; i < 30; i++) {
		for (std::size_t j = 0; j < 30; j++) {
			heightfield_data[j*30+i] = (sfImageGetPixelFloat(heightimage, i/30.0, 1-(j/30.0)).r/255.0)*64.0;
		}
	}
	btHeightfieldTerrainShape *ground_heightfield = new btHeightfieldTerrainShape(30, 30, heightfield_data, 1, 0, 64, 2, PHY_FLOAT, false);
	ground_heightfield->setUseDiamondSubdivision(true);
	ground_heightfield->setLocalScaling(btVector3(btScalar(64), btScalar(64), btScalar(1)));
	
	btTriangleMesh *mesh = new btTriangleMesh();
	transfer_bullet_shape(*mesh, obj_test);
	PhysicsShapeTypeMeshStatic *mesh_shape = new PhysicsShapeTypeMeshStatic();
	mesh_shape->set_mesh(mesh);
	
	btAlignedObjectArray<btVector3> verts;
	transfer_bullet_hull(verts, obj_test);
	btConvexHullShape *ground_hull = new btConvexHullShape(&(verts[0].getX()), verts.size());
	
	btBvhTriangleMeshShape *ground_shape = static_cast<btBvhTriangleMeshShape *>(mesh_shape->createShape());
	btCollisionShape *ground_box = new btBoxShape(btVector3(btScalar(30 * 64), btScalar(30 * 64), btScalar(1)));
	
	btTransform groundTrans;
	groundTrans.setIdentity();
//	groundTrans.setOrigin(btVector3(0, 0, 0));
	groundTrans.setOrigin(btVector3(15*64, 0, 32));
	btDefaultMotionState *grMotionState = new btDefaultMotionState(groundTrans);
	btRigidBody::btRigidBodyConstructionInfo grInfo(0., grMotionState, ground_heightfield);
	btRigidBody *grBody = new btRigidBody(grInfo);
	grBody->setRestitution(btScalar(1));
	grBody->setCollisionFlags(grBody->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
	grBody->setCollisionFlags(grBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
	btTriangleInfoMap* triangleInfoMap = new btTriangleInfoMap();
//	gContactAddedCallback = CustomMaterialCombinerCallback;
//	btGenerateInternalEdgeInfo(ground_shape, triangleInfoMap);
//	grBody->setContactProcessingThreshold(btScalar(64));

	auto ground_phy = new PhysicsObject(true);
	ground_phy->isCell = true;
	grBody->setUserPointer(ground_phy);
	Generic::PhysicsGeneral()->dynaWorld->addRigidBody(grBody);
}
