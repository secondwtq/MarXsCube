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

#include <vector>

extern Acheron::ThreadWorkerQueue queue_callback;

Grit *get_grit();

namespace Acheron {
	namespace Cube {
		
		void find_path_async(GPointType start, GPointType end, luabridge::CallbackRef callback) {
			
			async_dispatch<std::vector<GPointType> >(queue_callback,
				[start, end] () {
					std::vector<GPointType> t;
					for(std::size_t i = 0; i < 10000; i++)
						t = get_grit()->find_path(start, end);
					return t;
				},
				[callback] (const std::vector<GPointType>& vec) {
					callback(vec);
					callback.de_ref();
				});
			
		}
		
	}
}