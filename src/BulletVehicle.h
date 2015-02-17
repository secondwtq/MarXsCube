//
//  BulletVehicle.h
//  MarXsCube
//
//  Created by SeconDatke on 2/3/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef __MarXsCube__BulletVehicle__
#define __MarXsCube__BulletVehicle__

#include "Common.h"
#include <cstddef>

class PhysicsObject;

class BulletVehicle {
public:
	
	std::size_t tyre_count = 0;
	
	BulletVehicle(PhysicsObject *p) : parent(p) { }
	
	virtual void spawn() = 0;
	
	virtual void launch() = 0;
	
	virtual void add_wheel(const CoordStruct& location, float radius, float width) = 0;
	
	virtual void setup_wheels() = 0;
	
	virtual void clear_steer() = 0;
	
	virtual void clear_brake() = 0;
	
	virtual void brake_atonce() = 0;
	
	virtual void brake_atonce_force(float brake_force) = 0;
	
	virtual void brake_tyre_atonce(std::size_t wheel_id, float brake_force) = 0;
	
	virtual void launch_tyre(std::size_t wheel_id, float engine_force) = 0;
	
	virtual void set_steer(std::size_t wheel_id, float value) = 0;
	
	virtual void set_maxspeed(std::size_t wheel_id, float value) = 0;
	
	virtual void set_wheelfriction(float friction) = 0;
	
	virtual float get_current_speed() = 0;
	
	virtual ~BulletVehicle() { }
	
	PhysicsObject *parent = nullptr;
	
};

#include "Physics.h"

#endif /* defined(__MarXsCube__BulletVehicle__) */
