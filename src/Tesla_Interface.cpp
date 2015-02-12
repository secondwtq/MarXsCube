//
//  Tesla_Interface.cpp
//  MarXsCube
//
//  Created by SeconDatke on 2/5/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "Common.h"
#include "Generic.h"
#include "TeslaObject.h"
#include "TeslaRendering.h"
#include "Tesla_Interface.h"

void LuaInterface::RegisterInterface_Tesla(LuaStatus &L) {

	luabridge::getGlobalNamespace(L).
		beginNamespace("Tesla").
			beginClass<TeslaObject>("TeslaChunkObject").
				addFunction("location", &TeslaObject::location).
				addFunction("set_heightfield", &TeslaObject::set_heightfield).
				addFunction("set_tileset", &TeslaObject::set_tileset).
				addFunction("create_bullet", &TeslaObject::create_bullet).
				addFunction("load_objfile", &TeslaObject::load_objfile).
				addFunction("load_shader", &TeslaObject::load_shader).
				addFunction("load_buffer", &TeslaObject::load_buffer).
				addStaticFunction("create", &TeslaObject::create).
			endClass().
			beginClass<TeslaRenderingManger>("TeslaRenderingManger").
				addFunction("add_chunk", &TeslaRenderingManger::add_chunk).
			endClass().
		endNamespace().
		beginNamespace("Generic").
			addFunction("TeslaManger", &Generic::TeslaManger).
		endNamespace();
}