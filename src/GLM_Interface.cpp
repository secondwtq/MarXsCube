//
//  GLM_Interface.cpp
//  MarXsCube
//
//  Created by SeconDatke on 2/5/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "Config.h"
#include "GLM_Interface.h"

#include <glm/glm.hpp>

void LuaInterface::RegisterInterface_GLM(LuaStatus& L) {

	luabridge::getGlobalNamespace(L).
		beginNamespace("Utility").
			beginNamespace("glm").
				beginClass<glm::vec3>("gvec3").
					addConstructor<void (*)(float, float, float)>().
					addData("x", &glm::vec3::x).
					addData("y", &glm::vec3::y).
					addData("z", &glm::vec3::z).
				endClass().
			endNamespace().
		endNamespace();
	
}
