//
//  AcheronInterface.cpp
//  MarXsCube
//
//  Created by SeconDatke on 2/10/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "Config.h"
#include "AcheronCube.h"
#include "Acheron_Interface.h"

void LuaInterface::RegisterInterface_Acheron(LuaStatus& L) {
	luabridge::getGlobalNamespace(L).
		beginNamespace("Acheron").
			addFunction("find_path_async", &Acheron::Cube::find_path_async).
		endNamespace();
}