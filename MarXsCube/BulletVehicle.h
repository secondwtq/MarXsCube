//
//  BulletVehicle.h
//  MarXsCube
//
//  Created by SeconDatke on 2/2/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef __MarXsCube__BulletVehicle__
#define __MarXsCube__BulletVehicle__

#include "Common.h"

#include "PhysicsCommon.h"
#include "Physics.h"
#include "BulletDynamics/Vehicle/btRaycastVehicle.h"

class BulletVehicle {
public:
	
	static BulletVehicle *create() { return new BulletVehicle(); }
	
	void spawn();
	
	void launch();
	
	void add_wheel(const CoordStruct& location, float radius);
	
	void setup_wheels();
	
	PhysicsObject *parent = nullptr;
	
private:
	btVehicleRaycaster *m_vehicle_raycaster = nullptr;
	btRaycastVehicle *m_vehicle = nullptr;
	btRaycastVehicle::btVehicleTuning m_tuning;
};

#endif /* defined(__MarXsCube__BulletVehicle__) */
