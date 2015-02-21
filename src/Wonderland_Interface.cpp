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
			addFunction("set_texture", Wonderland::Foundation::set_texture).
			addFunction("set_texture_and_blend", Wonderland::Foundation::set_texture_and_blend).
			addFunction("buffer_update", Wonderland::Foundation::buffer_update).
			addFunction("blend_cells_batch", Wonderland::Foundation::blend_cells_batch).
			addFunction("seralize_chunk", Wonderland::Foundation::seralize_chunk).
		endNamespace();

}