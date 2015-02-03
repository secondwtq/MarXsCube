//
//  ConstraintVehicle.cpp
//  MarXsCube
//
//  Created by SeconDatke on 2/3/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "Common.h"
#include "PhysicsCommon.h"
#include "Generic.h"
#include "ConstraintVehicle.h"

void ConstraintVehicle::spawn() {
	this->parent->body->setActivationState(DISABLE_DEACTIVATION);
}

void ConstraintVehicle::launch() {
	for (std::size_t i = 0; i < this->m_count_tyres; i++) {
		btRotationalLimitMotor *motor = this->m_constraints[i]->getRotationalLimitMotor(0);
		motor->m_enableMotor = true;
		motor->m_maxMotorForce = 100.0f;
		motor->m_targetVelocity = -10.0f;
		
		motor = this->m_constraints[i]->getRotationalLimitMotor(2);
		motor->m_enableMotor = true;
		motor->m_loLimit = -0.1f;
		motor->m_hiLimit = 0.1f;
		motor->m_maxLimitForce = 100.0f;
	}
}

void ConstraintVehicle::setup_wheels() {
	
}

void ConstraintVehicle::brake_atonce() {
	for (std::size_t i = 0; i < 2; i++) {
		btRotationalLimitMotor *motor = this->m_constraints[i]->getRotationalLimitMotor(2);
		motor->m_enableMotor = true;
		motor->m_maxMotorForce = 100.0f;
		motor->m_targetVelocity = -10.0f;
		motor->m_loLimit = -1.0f;
		motor->m_hiLimit = 1.0f;
	}
}

void ConstraintVehicle::clear_steer() {
	for (std::size_t i = 0; i < this->m_count_tyres; i++) {
		
	}
}

btVector3 parent_axis { 0.f, 0.f, 1.f };
btVector3 child_axis { 0.f, 1.f, 0.f };

void ConstraintVehicle::add_wheel(const CoordStruct &location, float radius, float width) {
	float mass = 100.0;
	btVector3 final_location = this->parent->body->getCenterOfMassTransform().getOrigin() + coord2bt(location);
	btVector3 final_location_anchor = final_location;
//	final_location_anchor.setZ(final_location.getZ() + 64 * PHY_SCALE);
	
	btTransform tr;
	tr.setIdentity();
	tr.setOrigin(final_location);
	
	btCollisionShape *tyre_shape = new btCylinderShape(btVector3(btScalar(radius*PHY_SCALE), btScalar((width/2.0)*PHY_SCALE), btScalar(radius*PHY_SCALE)));
	btVector3 local_intertia { 0, 0, 0 };
	tyre_shape->calculateLocalInertia(mass, local_intertia);
	
	btDefaultMotionState* motion_state = new btDefaultMotionState(tr);
	btRigidBody::btRigidBodyConstructionInfo rb_info(mass, motion_state, tyre_shape, local_intertia);
	btRigidBody *tyre_body = new btRigidBody(rb_info);
	tyre_body->setFriction(btScalar(2.0));
	tyre_body->setUserPointer(nullptr);
	Generic::PhysicsGeneral()->dynaWorld->addRigidBody(tyre_body);
	tyre_body->setActivationState(DISABLE_DEACTIVATION);
	
	btHinge2Constraint *constraint = new btHinge2Constraint(*this->parent->body, *tyre_body,
															final_location_anchor, parent_axis, child_axis);
//	constraint->setUpperLimit(0);
//	constraint->setLowerLimit(0);
	
	Generic::PhysicsGeneral()->dynaWorld->addConstraint(constraint, true);
	constraint->setDbgDrawSize(btScalar(3.f));
	
	this->m_count_tyres++;
	this->m_constraints.push_back(constraint);
	this->m_tyres.push_back(tyre_body);
}