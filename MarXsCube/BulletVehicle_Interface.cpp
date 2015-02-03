//
//  BulletVehicle_Interface.cpp
//  MarXsCube
//
//  Created by SeconDatke on 2/3/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "Config.h"

#include "BulletVehicle.h"
#include "RaycastVehicle.h"
#include "ConstraintVehicle.h"

#include "BulletVehicle_Interface.h"

void LuaInterface::RegisterInterface_Physics_Vehicle(LuaStatus &L) {
	luabridge::getGlobalNamespace(L).
	beginNamespace("Physics").
		beginClass<BulletVehicle>("BulletVehicle").
			addData("parent", &BulletVehicle::parent).
			addFunction("spawn", &BulletVehicle::spawn).
			addFunction("setup_wheels", &BulletVehicle::setup_wheels).
			addFunction("add_wheel", &BulletVehicle::add_wheel).
			addFunction("launch", &BulletVehicle::launch).
			addFunction("clear_steer", &BulletVehicle::clear_steer).
			addFunction("brake_atonce", &BulletVehicle::brake_atonce).
		endClass().
		deriveClass<RaycastVehicle, BulletVehicle>("RaycastVehicle").
			addStaticFunction("create", &RaycastVehicle::create).
		endClass().
	endNamespace();
}