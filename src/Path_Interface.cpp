//
//  Path_Interface.cpp
//  MarXsCube
//
//  Created by SeconDatke on 2/19/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "Config.h"
#include "Path.h"
#include "Pathway.h"
#include "PathCube.h"
#include "Path_Interface.h"

void LuaInterface::RegisterInterface_Path(LuaStatus& L) {
	luabridge::getGlobalNamespace(L).
		beginNamespace("Utility").
		beginNamespace("Path").
			beginClass<Path>("Path").
				addFunction("mapPathDistanceToPoint", &Path::mapPathDistanceToPoint).
				addFunction("mapPointToPathDistance", &Path::mapPointToPathDistance).
			endClass().
			beginClass<Pathway>("Pathway").
				addFunction("mapPathDistanceToPoint", &Pathway::mapPathDistanceToPoint).
				addFunction("mapPointToPathDistance", &Pathway::mapPointToPathDistance).
			endClass().
			addFunction("path_from_gritpath", &path_from_gritpath).
		endNamespace().
	endNamespace();
}