//
//  Generic.cpp
//  MarXsCube
//
//  Created by SeconDatke on 14-9-22.
//  Copyright (c) 2014年 MarXsCube Staff. All rights reserved.
//

#include "Common.h"

#include "Session.h"
#include "ObjectManger.h"
#include "PhysicsGeneral.h"
#include "Config.h"
#include "LuaJit_Bridge.h"

#include "Generic.h"

class Session *Generic::session = nullptr;
class RenderLayerManger *Generic::render_layer_manger = nullptr;
class PhysicsGeneral *Generic::physics_general = nullptr;
class luabridge::LuaRef *Generic::foo_object_table_create;

class FSM::FSMLoggerProxy Generic::core_logger = FSM::create_handle();
class LuaStatus *Generic::state = nullptr;

void Generic::Init_PhysicsGeneral() {
	Generic::physics_general = new class PhysicsGeneral(); }

void Generic::Dispose_PhysicsGeneral() {
	Generic::physics_general->dispose();
	delete Generic::physics_general;
	Generic::physics_general = nullptr;
}

void Generic::Init_FunObjectTableCreate_Forward(ConfigManger& config_manger) {
	Generic::foo_object_table_create = new luabridge::LuaRef(*config_manger.ConfigState, luabridge::Nil());
	*Generic::foo_object_table_create = luabridge::getGlobal(*config_manger.ConfigState, "YouFuckingNULLPOINTER");
}

void Generic::Init_FunObjectTableCreate(ConfigManger& config_manger) {
	Generic::foo_object_table_create = new luabridge::LuaRef(*config_manger.ConfigState, luabridge::Nil());
	*Generic::foo_object_table_create = luabridge::getGlobal(*config_manger.ConfigState, "CubeCore_CreateObjectTable");
}