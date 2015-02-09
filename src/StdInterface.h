//
//  StdInterface.h
//  MarXsCube
//
//  Created by SeconDatke on 14-12-10.
//  Copyright (c) 2014年 MarXsCube Staff. All rights reserved.
//

#ifndef __MarXsCube__StdInterface__
#define __MarXsCube__StdInterface__

#include "Common.h"
#include "Config.h"

namespace LuaInterface {
	
	template <typename ValueT>
	void RegisterInterface_Std(LuaStatus &L, const char *name_type);
	
}

#endif /* defined(__MarXsCube__StdInterface__) */
