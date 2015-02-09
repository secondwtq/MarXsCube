#ifndef CUBE_E_PHY_MOTIONSTEST_H
#define CUBE_E_PHY_MOTIONSTEST_H

#include "Object.h"
#include "Physics.h"

ATTRIBUTE_ALIGNED16(struct) MoState : public btDefaultMotionState {
public:
	PhysicsObject *object_ptr = nullptr;
	MoState(const btTransform& startTrans = btTransform::getIdentity(), PhysicsObject *object = nullptr, const btTransform& centerOfMassOffset = btTransform::getIdentity());

	void getWorldTransform(btTransform &centerOfMassWorldTrans) const;

	void setWorldTransform(const btTransform &centerOfMassWorldTrans);
};

#endif