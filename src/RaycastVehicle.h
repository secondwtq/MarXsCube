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
	
	virtual void brake_atonce_force(float brake_force) { }
	
	virtual void brake_tyre_atonce(std::size_t wheel_id, float brake_force);
	
	virtual void set_steer(std::size_t wheel_id, float value);
	
	virtual void set_maxspeed(std::size_t wheel_id, float value) { }
	
	virtual void launch_tyre(std::size_t wheel_id, float engine_force);
	
	virtual ~RaycastVehicle() { }
	
private:
	btVehicleRaycaster *m_vehicle_raycaster = nullptr;
	btRaycastVehicle *m_vehicle = nullptr;
	btRaycastVehicle::btVehicleTuning m_tuning;
};

#endif /* defined(__MarXsCube__RaycastVehicle__) */
