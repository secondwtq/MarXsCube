//
//  ConstraintVehicle.h
//  MarXsCube
//
//  Created by SeconDatke on 2/3/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef __MarXsCube__ConstraintVehicle__
#define __MarXsCube__ConstraintVehicle__

#include "Common.h"

#include "PhysicsCommon.h"
#include "BulletVehicle.h"

#include <vector>

class ConstraintVehicle : public BulletVehicle {
	
	public:
		static ConstraintVehicle *create(PhysicsObject *parent) {
			return new ConstraintVehicle(parent); }
	
		ConstraintVehicle(PhysicsObject *parent) : BulletVehicle(parent) { }
	
		virtual void spawn();
		
		virtual void launch();
		
		virtual void add_wheel(const CoordStruct& location, float radius, float width);
		
		virtual void setup_wheels();
		
		virtual void clear_steer();
		
		virtual void brake_atonce();
	
		virtual void brake_tyre_atonce(std::size_t wheel_id, float brake_force);
	
		virtual void launch_tyre(std::size_t wheel_id, float engine_force);
	
		virtual void set_steer(std::size_t wheel_id, float value);
	
		virtual void set_maxspeed(std::size_t wheel_id, float value);
	
		virtual void brake_atonce_force(float brake_force);
	
		virtual float get_current_speed() { return 0.f; }
	
		virtual void set_wheelfriction(float friction) { }
	
		virtual ~ConstraintVehicle() { }
	
	private:
	
		std::vector<btRigidBody *> m_tyres;
		std::vector<btHinge2Constraint *> m_constraints;
	
};

#endif /* defined(__MarXsCube__ConstraintVehicle__) */
