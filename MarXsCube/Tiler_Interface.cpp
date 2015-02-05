//
//  Tiler_Interface.cpp
//  MarXsCube
//
//  Created by SeconDatke on 2/5/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "Common.h"
#include "TilerObject.h"
#include "Tiler_Interface.h"

void LuaInterface::RegisterInterface_Tiler(LuaStatus &L) {

	luabridge::getGlobalNamespace(L).
		beginNamespace("Tiler").
			beginClass<TilerObject>("TilerChunkObject").
				addConstructor<void (*)()>().
				addFunction("location", &TilerObject::location).
				addFunction("set_heightfield", &TilerObject::set_heightfield).
				addFunction("set_tileset", &TilerObject::set_tileset).
				addFunction("create_bullet", &TilerObject::create_bullet).
				addFunction("load_objfile", &TilerObject::load_objfile).
				addFunction("load_shader", &TilerObject::load_shader).
				addFunction("load_buffer", &TilerObject::load_buffer).
				addStaticFunction("create", &TilerObject::create).
			endClass().
		endNamespace();
}