//
//  RayTest_Interface.cpp
//  MarXsCube
//
//  Created by SeconDatke on 14-12-11.
//  Copyright (c) 2014年 MarXsCube Staff. All rights reserved.
//

#include "Common.h"
#include "Config.h"
#include "RayTest.h"
#include "RayTest_Interface.h"

using namespace Physics::RayTest;

void LuaInterface::RegisterInterface_Physics_RayTest(LuaStatus &L) {
	luabridge::getGlobalNamespace(L).
	beginNamespace("Physics").
		// well, I don't know why so many interface to the creator methods is needed.
		addFunction("createRayTestSingle", &Physics::RayTest::createRayTestSingle).
		addFunction("createRayTestForCell", &Physics::RayTest::createRayTestForCell).
		addFunction("createRayTestTechnoOnly", &Physics::RayTest::createRayTestTechnoOnly).
		beginClass<RayTestSingle>("RayTestSingle").
			addConstructor<void (*)(const CoordStruct&, const CoordStruct&)>().
			addFunction("perform", &RayTestSingle::perform).
			addFunction("getFirstObject", &RayTestSingle::getFirstObject).
			addFunction("hit", &RayTestSingle::hit).
			addFunction("hit_point", &RayTestSingle::hit_point).
			addStaticFunction("createRayTestSingle", &Physics::RayTest::createRayTestSingle).
			addStaticFunction("create", &Physics::RayTest::createRayTestSingle).
		endClass().

		beginClass<RayTestSingleForCell>("RayTestSingleForCell").
			addConstructor<void (*)(const CoordStruct&, const CoordStruct&)>().
			addFunction("perform", &RayTestSingleForCell::perform).
			addFunction("getFirstObject", &RayTestSingleForCell::getFirstObject).
			addFunction("hit", &RayTestSingleForCell::hit).
			addFunction("hit_point", &RayTestSingleForCell::hit_point).
			addStaticFunction("createRayTestForCell", &Physics::RayTest::createRayTestForCell).
			addStaticFunction("create", &Physics::RayTest::createRayTestForCell).
		endClass().
	
		beginClass<_impl::RayTestSingleTechnoOnly>("_RayTestSingleTechnoOnly").
			addConstructor<void (*)(const CoordStruct&, const CoordStruct&)>().
			addFunction("perform", &_impl::RayTestSingleTechnoOnly::perform).
			addFunction("getFirstObject", &_impl::RayTestSingleTechnoOnly::getFirstObject).
			addFunction("hit", &_impl::RayTestSingleTechnoOnly::hit).
			addFunction("hit_point", &_impl::RayTestSingleTechnoOnly::hit_point).
		endClass().

		deriveClass<RayTestSingleTechnoOnly, _impl::RayTestSingleTechnoOnly>("RayTestSingleTechnoOnly").
			addConstructor<void (*)(const CoordStruct&, const CoordStruct&)>().
			addFunction("set_except", &RayTestSingleTechnoOnly::set_except).
			addFunction("perform", &RayTestSingleTechnoOnly::perform).
			addFunction("getFirstObject", &RayTestSingleTechnoOnly::getFirstObject).
			addFunction("hit", &RayTestSingleTechnoOnly::hit).
			addFunction("hit_point", &RayTestSingleTechnoOnly::hit_point).
			addStaticFunction("create", &Physics::RayTest::createRayTestTechnoOnly).
		endClass().

	endNamespace();
}
