//
//  Generic.h
//  MarXsCube
//
//  Created by SeconDatke on 14-9-22.
//  Copyright (c) 2014年 MarXsCube Staff. All rights reserved.
//

#ifndef __MarXsCube__Generic__
#define __MarXsCube__Generic__

#include "Common.h"
#include "ObjectManger.h"
#include "Session.h"
#include "Config.h"

class PhysicsGeneral;

class Generic {
public:

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-fpermissive"
#endif
	static inline Session *Session() {
		return Generic::session;
	}
	
	static inline RenderLayerManger *RenderLayerManger() {
		return Generic::render_layer_manger;
	}
	
	static inline void Init_Session() {
		Generic::session = new class Session();
	}
	
	static inline void Init_RenderLayerManger() {
		Generic::render_layer_manger = new class RenderLayerManger();
	}
 
	static inline void RemoveObject(Abs_Abstract *src) {
		Generic::Session()->removeObject(src);
	}
	
	static inline PhysicsGeneral* PhysicsGeneral() {
		return Generic::physics_general;
	}
	
	static inline void Init_PhysicsGeneral() {
		Generic::physics_general = new class PhysicsGeneral();
	}
	
	static inline void Dispose_PhysicsGeneral() {
		Generic::physics_general->dispose();
		delete Generic::physics_general;
		Generic::physics_general = nullptr;
	}
	
	static void Init_FunObjectTableCreate(ConfigManger& config_manger);
	
	static void Init_FunObjectTableCreate_Forward(ConfigManger& config_manger);
	
	static inline luabridge::LuaRef CreateObjectTable(unsigned int id) {
		return (*Generic::foo_object_table_create)(id);
	}
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif
	
public:
	static class Session *session;
	static class RenderLayerManger *render_layer_manger;
	
	static class PhysicsGeneral *physics_general;
	
	static class luabridge::LuaRef *foo_object_table_create;
};

#include "PhysicsGeneral.h"

#endif /* defined(__MarXsCube__Generic__) */
