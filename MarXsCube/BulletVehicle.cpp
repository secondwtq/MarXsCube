//
//  BulletVehicle.cpp
//  MarXsCube
//
//  Created by SeconDatke on 2/2/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "Common.h"

#include "BulletDynamics/Vehicle/btRaycastVehicle.h"
#include "Generic.h"
#include "BulletVehicle.h"

const btVector3 wheel_direction { 0, 0, -1 };
const btVector3 wheel_axis { 0, -1, 0 };

void BulletVehicle::spawn() {
	this->m_vehicle_raycaster = new btDefaultVehicleRaycaster(Generic::PhysicsGeneral()->dynaWorld);
	this->m_vehicle = new btRaycastVehicle(this->m_tuning, this->parent->body, this->m_vehicle_raycaster);
	Generic::PhysicsGeneral()->dynaWorld->addVehicle(this->m_vehicle);
	this->parent->body->setActivationState(DISABLE_DEACTIVATION);
	m_vehicle->setCoordinateSystem(0, 2, 1);
}

namespace {
	float wheelFriction = 100000.0;
	float suspensionStiffness = 10.f;
	float suspensionDamping = 1.3f;
	float suspensionCompression = 4.4f;
	float rollInfluence = 0.8f;
	btScalar suspensionRestLength(3.0);
}

void BulletVehicle::add_wheel(const CoordStruct& location, float radius) {
	this->m_vehicle->addWheel(btVector3(location.x, location.y, location.z), wheel_direction, wheel_axis, suspensionRestLength, btScalar(radius), this->m_tuning, true);
}

void BulletVehicle::setup_wheels() {
	for (int i = 0; i < this->m_vehicle->getNumWheels(); i++) {
		btWheelInfo& wheel = m_vehicle->getWheelInfo(i);
		wheel.m_suspensionStiffness = suspensionStiffness;
		wheel.m_wheelsDampingRelaxation = suspensionDamping;
		wheel.m_wheelsDampingCompression = suspensionCompression;
		wheel.m_frictionSlip = wheelFriction;
		wheel.m_rollInfluence = rollInfluence;
		
		wheel.m_maxSuspensionForce = 1000000.0f;
	}
}

void BulletVehicle::launch() {
	for (int i = 0; i < this->m_vehicle->getNumWheels(); i++) {
		m_vehicle->applyEngineForce(120, i);
		m_vehicle->setBrake(2000, i);
		m_vehicle->setSteeringValue(0, i);
	}
}

void BulletVehicle::clear_steer() {
	for (int i = 0; i < this->m_vehicle->getNumWheels(); i++) {
		m_vehicle->setSteeringValue(0, i);
//		m_vehicle->setBrake(500, i);
	}
}