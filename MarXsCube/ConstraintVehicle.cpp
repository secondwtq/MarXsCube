//
//  ConstraintVehicle.cpp
//  MarXsCube
//
//  Created by SeconDatke on 2/3/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "ConstraintVehicle.h"

void ConstraintVehicle::spawn() {
	this->parent->body->setActivationState(DISABLE_DEACTIVATION);
}

void ConstraintVehicle::launch() {
	for (std::size_t i = 0; i < this->m_count_tyres; i++) {
		
	}
}

void ConstraintVehicle::setup_wheels() {
	
}

void ConstraintVehicle::brake_atonce() {
	for (std::size_t i = 0; i < this->m_count_tyres; i++) {
		
	}
}

void ConstraintVehicle::clear_steer() {
	for (std::size_t i = 0; i < this->m_count_tyres; i++) {
		
	}
}

void ConstraintVehicle::add_wheel(const CoordStruct &location, float radius, float width) {
	
}