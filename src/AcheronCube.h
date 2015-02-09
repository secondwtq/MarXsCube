//
//  AcheronCube.h
//  MarXsCube
//
//  Created by SeconDatke on 2/10/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef __MarXsCube__AcheronCube__
#define __MarXsCube__AcheronCube__

#include "Common.h"
#include "Config.h"
#include "GritFoundation.h"
#include "CallbackRef.h"

namespace Acheron {
	namespace Cube {
		
		void find_path_async(GPointType start, GPointType end, lua_State* L);
		
	}
}

#endif /* defined(__MarXsCube__AcheronCube__) */
