//
//  Wonderland_Interface.cpp
//  MarXsCube
//
//  Created by SeconDatke on 2/15/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "Config.h"
#include "WonderlandFoundation.h"
#include "Wonderland_Interface.h"

void LuaInterface::RegisterInterface_Wonderland(LuaStatus &L) {
	
	luabridge::getGlobalNamespace(L).
		beginNamespace("Wonderland").
			addFunction("set_texture_and_blend", Wonderland::Foundation::set_texture_and_blend).
		endNamespace();

}