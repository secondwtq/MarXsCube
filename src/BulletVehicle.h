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

class PhysicsObject;

class BulletVehicle {
public:
	
	virtual void spawn() = 0;
	
	virtual void launch() = 0;
	
	virtual void add_wheel(const CoordStruct& location, float radius, float width) = 0;
	
	virtual void setup_wheels() = 0;
	
	virtual void clear_steer() = 0;
	
	virtual void brake_atonce() = 0;
	
	virtual ~BulletVehicle() { }
	
	PhysicsObject *parent = nullptr;
	
};

#include "Physics.h"

#endif /* defined(__MarXsCube__BulletVehicle__) */
