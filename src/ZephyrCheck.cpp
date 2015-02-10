//
//  ZephyrCheck.cpp
//  MarXsCube
//
//  Created by SeconDatke on 2/10/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "Common.h"
#include "HafniumCommon.h"
#include "ZephyrCommon.h"

#include "Abstract.h"
#include "Anim.h"
#include "Object.h"
#include "Techno.h"
#include "ZephyrCore.h"

#include "ZephyrCheck.h"

#include <cstdio>
#include <vector>

void zephyr_check_output() {
	ZEPHYR_PLACEHOLDER_VECTOR vec_t;
	printf("vector: %lu %lu\n", sizeof(std::vector<int>), sizeof(vec_t));
	printf("CoordStruct: %lu %lu\n", sizeof(CoordStruct), sizeof(ZCoordStruct));
	printf("LuaRef: %lu %lu\n", sizeof(luabridge::LuaRef), sizeof(ZephyrScriptRef));
	printf("Abs_Abstract: %lu %lu\n", sizeof(Abs_Abstract), sizeof(ZephyrObject));
	printf("Abs_Anim: %lu %lu\n", sizeof(Abs_Anim), sizeof(ZephyrAnim));
	printf("Abs_Object: %lu %lu\n", sizeof(Abs_Object), sizeof(ZephyrObject_));
	printf("Abs_Techno: %lu %lu\n", sizeof(Abs_Techno), sizeof(ZephyrTechno));
}