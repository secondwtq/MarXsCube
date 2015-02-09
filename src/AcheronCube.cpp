//
//  AcheronCube.cpp
//  MarXsCube
//
//  Created by SeconDatke on 2/10/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "Common.h"
#include "AcheronGCD.h"
#include "GritFoundation.h"
#include "AcheronCube.h"

extern Acheron::ThreadWorkerQueue queue_callback;

Grit *get_grit();

namespace Acheron {
	namespace Cube {
		
		void find_path_async(GPointType start, GPointType end, lua_State *L) {
			unsigned int ref = lua_ref(L, LUA_REGISTRYINDEX);
			async_dispatch(queue_callback, [start, end] () { for(std::size_t i = 0; i < 1000; i++) get_grit()->find_path(start, end); }, [ref, L] () { lua_rawgeti(L, LUA_REGISTRYINDEX, ref); luabridge::LuaException::pcall(L, 0, 1); });
			
		}
		
	}
}