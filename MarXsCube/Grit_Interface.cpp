//
//  Grit_Interface.cpp
//  MarXsCube
//
//  Created by SeconDatke on 2/6/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "Config.h"
#include "GritFoundation.h"
#include "Grit_Interface.h"

void LuaInterface::RegisterInterface_Grit(LuaStatus& L) {
	luabridge::getGlobalNamespace(L).
		beginNamespace("Grit").
			beginClass<Grit>("Grit").
	
			endClass().
		endNamespace();
}
