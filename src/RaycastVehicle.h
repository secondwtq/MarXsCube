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

#define KMHTOMS (0.277777778f)

class RaycastVehicle : public BulletVehicle {
public:
	
	static RaycastVehicle *create(PhysicsObject *parent) {
		return new RaycastVehicle(parent); }
	
	RaycastVehicle(PhysicsObject *parent) : BulletVehicle(parent) { }
	
	void spawn();
	
	void launch();
	
	void add_wheel(const CoordStruct& location, float radius, float width);
	
	void setup_wheels();
	
	void clear_steer();
	
	virtual void clear_brake();
	
	void brake_atonce();
	
	virtual void brake_atonce_force(float brake_force) { }
	
	virtual void brake_tyre_atonce(std::size_t wheel_id, float brake_force);
	
	virtual void set_steer(std::size_t wheel_id, float value);
	
	virtual void set_maxspeed(std::size_t wheel_id, float value) { }
	
	virtual void launch_tyre(std::size_t wheel_id, float engine_force);
	
	virtual float get_current_speed() {
		return this->m_vehicle->getCurrentSpeedKmHour() * KMHTOMS; }
	
	virtual void set_wheelfriction(float friction) {
		this->wheel_friction = friction; }
	
	virtual ~RaycastVehicle() { }
	
private:
	
	float wheel_friction = 2.0f;
	
	btVehicleRaycaster *m_vehicle_raycaster = nullptr;
	btRaycastVehicle *m_vehicle = nullptr;

};

#endif /* defined(__MarXsCube__RaycastVehicle__) */
