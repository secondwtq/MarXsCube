//
//  BulletVehicle.h
//  MarXsCube
//
//  Created by SeconDatke on 2/2/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef __MarXsCube__RaycastVehicle__
#define __MarXsCube__RaycastVehicle__

#include "Common.h"

#include "PhysicsCommon.h"
#include "Physics.h"
#include "BulletDynamics/Vehicle/btRaycastVehicle.h"

class RaycastVehicle : public BulletVehicle {
public:
	
	static RaycastVehicle *create() { return new RaycastVehicle(); }
	
	void spawn();
	
	void launch();
	
	void add_wheel(const CoordStruct& location, float radius, float width);
	
	void setup_wheels();
	
	void clear_steer();
	
	void brake_atonce();
	
	virtual ~RaycastVehicle() { }
	
private:
	btVehicleRaycaster *m_vehicle_raycaster = nullptr;
	btRaycastVehicle *m_vehicle = nullptr;
	btRaycastVehicle::btVehicleTuning m_tuning;
};

#endif /* defined(__MarXsCube__RaycastVehicle__) */
