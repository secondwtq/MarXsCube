#ifndef CUBE_E_PHYSICSCHECK_H
#define CUBE_E_PHYSICSCHECK_H

#include "Common.h"

class Abs_Cell;

struct ContactSensorCallback_CellCheck : public btCollisionWorld::ContactResultCallback {

	ContactSensorCallback_CellCheck(btRigidBody& tgtBody , Abs_Cell *_cell);

	btRigidBody& body;
	Abs_Cell *cell;

	virtual bool needsCollision(btBroadphaseProxy* proxy) const {
		// // superclass will check m_collisionFilterGroup and m_collisionFilterMask
		// if(!btCollisionWorld::ContactResultCallback::needsCollision(proxy))
		// 	return false;
		return body.checkCollideWithOverride(static_cast<btCollisionObject*>(proxy->m_clientObject));
	}

	virtual btScalar addSingleResult(btManifoldPoint& cp,
		const btCollisionObjectWrapper* colObj0,int partId0,int index0,
		const btCollisionObjectWrapper* colObj1,int partId1,int index1);
};

struct ContactSensorCallback_CellCheck_ : public btCollisionWorld::ContactResultCallback {

	ContactSensorCallback_CellCheck_(btRigidBody& tgtBody , bool *_ret, Abs_Abstract *Obj)
		: btCollisionWorld::ContactResultCallback(), body(tgtBody), ret(_ret), obj(Obj) { }

	btRigidBody& body;
	bool *ret;
	Abs_Abstract *obj;

	virtual bool needsCollision(btBroadphaseProxy* proxy) const {
		if ( ((PhysicsObject *) (((btCollisionObject *)(proxy->m_clientObject)) ->getUserPointer())) ->isEmpty ) return false;
		return body.checkCollideWithOverride(static_cast<btCollisionObject*>(proxy->m_clientObject));
	}

	virtual btScalar addSingleResult(btManifoldPoint& cp,
		const btCollisionObjectWrapper* colObj0,int partId0,int index0,
		const btCollisionObjectWrapper* colObj1,int partId1,int index1) {
			Abs_Abstract *targetPtr = nullptr;
			if(colObj0->m_collisionObject==&body) targetPtr = ((PhysicsObject *)((btCollisionObject *)colObj1->m_collisionObject->getUserPointer()))->attachedToObject;
			else targetPtr = ((PhysicsObject *)((btCollisionObject *)colObj0->m_collisionObject->getUserPointer()))->attachedToObject;
			if (targetPtr != obj) *ret = false;
		return 0;
	}
};

#include "Cell.h"

#endif