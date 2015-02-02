//
//  BulletVehicle.cpp
//  MarXsCube
//
//  Created by SeconDatke on 2/2/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "BulletDynamics/Vehicle/btRaycastVehicle.h"
#include "Generic.h"
#include "BulletVehicle.h"

void BulletVehicle::spawn() {
	this->m_vehicle_raycaster = new btDefaultVehicleRaycaster(Generic::PhysicsGeneral()->dynaWorld);
	this->m_vehicle = new btRaycastVehicle(this->m_tuning, this->parent->body, this->m_vehicle_raycaster);
	Generic::PhysicsGeneral()->dynaWorld->addVehicle(this->m_vehicle);
	this->parent->body->setActivationState(DISABLE_DEACTIVATION);
	m_vehicle->setCoordinateSystem(0, 2, 1);
}