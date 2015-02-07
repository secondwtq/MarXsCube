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
#include "FSM.h"
#include "TeslaRendering.h"

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
		return Generic::render_layer_manger; }
	
	static inline TeslaRenderingManger *TeslaRenderingManger() {
		return Generic::m_teslamanger; }
	
	static inline void Init_Logger() {
		FSM::init();
		core_logger = FSM::logger("CubeCore").set_deflogger().get_proxy();
		FSM::logger(FSMHelper::L::Debug) << "FSM ready." << FSMHelper::rn;
	}
	
	static inline FSM::FSMLoggerProxy& corelog() { return core_logger; }
	
	static inline void Dispose_Logger_n_FSM() {
		FSM::dispose_logger("CubeCore");
		return FSM::dispose();
	}
	
	static inline void Init_LuaStatus() {
		state = new LuaStatus();
		state->init();
	}
	
	static inline LuaStatus *lua_state() { return state; }
	
	static inline void Init_Session() {
		Generic::session = new class Session();
	}
	
	static inline void Init_RenderLayerManger() {
		Generic::render_layer_manger = new class RenderLayerManger(); }
	
	static inline void init_TeslaRenderingManger() {
		Generic::m_teslamanger = new class TeslaRenderingManger(); }
 
	static inline void RemoveObject(Abs_Abstract *src) {
		Generic::Session()->removeObject(src);
	}
	
	static inline PhysicsGeneral* PhysicsGeneral() {
		return Generic::physics_general; }
	
	static void Init_PhysicsGeneral();
	
	static void Dispose_PhysicsGeneral();
	
	static void Init_FunObjectTableCreate(ConfigManger& config_manger);
	
	static void Init_FunObjectTableCreate_Forward(ConfigManger& config_manger);
	
	static inline luabridge::LuaRef CreateObjectTable(unsigned int id) {
		return (*Generic::foo_object_table_create)(id);
	}
//#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop
//#endif
	
public:
	static class Session *session;
	static class RenderLayerManger *render_layer_manger;
	
	static class PhysicsGeneral *physics_general;
	
	static class luabridge::LuaRef *foo_object_table_create;
	
	static class LuaStatus *state;
	
	static class FSM::FSMLoggerProxy core_logger;
	
	static class TeslaRenderingManger *m_teslamanger;
};

#include "PhysicsGeneral.h"

#endif /* defined(__MarXsCube__Generic__) */
