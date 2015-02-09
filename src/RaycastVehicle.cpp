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
#include "RaycastVehicle.h"

const btVector3 wheel_direction { 0, 0, -1 };
const btVector3 wheel_axis { 0, -1, 0 };

void RaycastVehicle::spawn() {
	this->m_vehicle_raycaster = new btDefaultVehicleRaycaster(Generic::PhysicsGeneral()->dynaWorld);
	this->m_vehicle = new btRaycastVehicle(this->m_tuning, this->parent->body, this->m_vehicle_raycaster);
	Generic::PhysicsGeneral()->dynaWorld->addVehicle(this->m_vehicle);
	this->parent->body->setActivationState(DISABLE_DEACTIVATION);
	m_vehicle->setCoordinateSystem(0, 2, 1);
}

namespace {
	float wheelFriction = 1.0f;
	float suspensionStiffness = 10.f;
	float suspensionDamping = .2 * 2 * std::sqrt(10);
	float suspensionCompression = .35 * 2 * std::sqrt(10);
	float rollInfluence = 1.0f;
	btScalar suspensionRestLength(1.0);
}

void RaycastVehicle::add_wheel(const CoordStruct& location, float radius, float width) {
	this->m_vehicle->addWheel(coord2bt(location), wheel_direction, wheel_axis, suspensionRestLength, btScalar(radius*PHY_SCALE), this->m_tuning, true);
}

void RaycastVehicle::setup_wheels() {
	for (int i = 0; i < this->m_vehicle->getNumWheels(); i++) {
		btWheelInfo& wheel = m_vehicle->getWheelInfo(i);
		wheel.m_suspensionStiffness = suspensionStiffness;
		wheel.m_wheelsDampingRelaxation = suspensionDamping;
		wheel.m_wheelsDampingCompression = suspensionCompression;
		wheel.m_frictionSlip = wheelFriction;
		wheel.m_rollInfluence = rollInfluence;
		
		//The wheels sink through the ground when the suspension cannot support the weight of the vehicle.
		// You need to increase the suspension stiffness, max travel or suspension length.
		// *Increasing the suspension too much will make the simulation unstable.*
		wheel.m_maxSuspensionForce = 10000.0f;
	}
}

void RaycastVehicle::launch() {
	for (int i = 0; i < this->m_vehicle->getNumWheels(); i++) {
		m_vehicle->applyEngineForce(10000, i);
//		m_vehicle->setBrake(2000, i);
//		m_vehicle->setSteeringValue(1, i);
	}
}

void RaycastVehicle::clear_steer() {
	for (int i = 0; i < this->m_vehicle->getNumWheels(); i++) {
		m_vehicle->setSteeringValue(0, i);
//		m_vehicle->setBrake(500, i);
	}
}

void RaycastVehicle::brake_atonce() {
	for (int i = 0; i < this->m_vehicle->getNumWheels(); i++) {
		m_vehicle->setBrake(2000, i);
		m_vehicle->setSteeringValue(0, i);
	}
}