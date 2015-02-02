//
//  BulletThread.cpp
//  MarXsCube
//
//  Created by SeconDatke on 1/24/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "Common.h"

#include "Generic.h"
#include "Acheron.h"

#include "BulletThread.h"

void bullet_acheron_function() {
	Generic::PhysicsGeneral()->dynaWorld->stepSimulation(1.f/(float)30, 24,
														 btScalar(1.)/120.0);
}

Acheron::AcheronBase Acheron::Bullet(bullet_acheron_function);

