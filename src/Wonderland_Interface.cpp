//
//  Wonderland_Interface.cpp
//  MarXsCube
//
//  Created by SeconDatke on 2/15/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "Config.h"
#include "WonderlandFoundation.h"
#include "WonderlandBrush.h"
#include "Wonderland_Interface.h"

void LuaInterface::RegisterInterface_Wonderland(LuaStatus &L) {
	
	luabridge::getGlobalNamespace(L).
		beginNamespace("Evan").
			beginClass<EvanDrawBuffer>("DrawBuffer").
				addConstructor<void (*)(std::size_t, std::size_t)>().
			endClass().
		endNamespace().
		beginNamespace("Wonderland").
			addFunction("set_texture", Wonderland::Foundation::set_texture).
			addFunction("set_texture_and_blend", Wonderland::Foundation::set_texture_and_blend).
			addFunction("buffer_update", Wonderland::Foundation::buffer_update).
			addFunction("blend_cells_batch", Wonderland::Foundation::blend_cells_batch).
			addFunction("serialize_chunk", Wonderland::Foundation::serialize_chunk).
			addFunction("deserialize_chunk", Wonderland::Foundation::deserialize_chunk).
			addFunction("brush_init", BrushInit).
			addFunction("brush_test", BrushTest).
		endNamespace();

}