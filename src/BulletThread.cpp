//
//  BulletThread.cpp
//  MarXsCube
//
//  Created by SeconDatke on 1/24/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "Common.h"

#include "ATVBCube.h"
#include "Generic.h"
#include "Acheron.h"

#include "BulletThread.h"

using namespace ATVBCube::Helper;

void bullet_acheron_function() {
	static float interval = (1.f / ATVBCube::setting<S::FPSLimitSetting>().limit_main) * ATVBCube::setting<S::BulletGeneralSetting>().boost;
	Generic::PhysicsGeneral()->dynaWorld->stepSimulation(interval, 24,
														 btScalar(1.)/120.0);
}

Acheron::AcheronBase Acheron::Bullet(bullet_acheron_function);

