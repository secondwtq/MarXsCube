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
		
		void find_path_async(GPointType start, GPointType end, luabridge::LuaRef callback) {
			async_dispatch(queue_callback, [start, end] () { for(std::size_t i = 0; i < 1000; i++) get_grit()->find_path(start, end); }, [callback] () { callback(); }); }
		
	}
}