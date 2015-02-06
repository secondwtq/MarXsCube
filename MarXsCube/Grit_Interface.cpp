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

Grit *get_grit();

void LuaInterface::RegisterInterface_Grit(LuaStatus& L) {
	luabridge::getGlobalNamespace(L).
		beginNamespace("Grit").
			addFunction("instance", &get_grit).
			beginClass<Grit>("Grit").
				addFunction("find_path", &Grit::find_path).
			endClass().
		endNamespace();
}
