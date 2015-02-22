//
//  ZephyrCore.h
//  MarXsCube
//
//  Created by SeconDatke on 2/10/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef MarXsCube_ZephyrCore_h
#define MarXsCube_ZephyrCore_h

#include "ZephyrCommon.h"

// original: Abs_Abstract
struct ZephyrObject {
	ZEPHYR_VTABLE;
	
	enum ZRenderLayerType_Type Layer;
	unsigned int RTTIID;
	
	bool useCollSphere;
	int rCollSphere;
	UNDEFINED_PTR *collSphere;
	bool EnablePhysics;
	UNDEFINED_PTR *Physics;
	
	ZephyrScriptRef ExtTable;
};

// original: Abs_Anim
struct ZephyrAnim {
	struct ZephyrObject parent;
	UNDEFINED_PTR AnimType;
	ZCubeTimer AnimTimer;
};

// original: Abs_Object
struct ZephyrObject_ {
	struct ZephyrObject parent;
	
	UNDEFINED_PTR Type;
	ZCoordStruct destnation;
	ZDirection direction;
	ZEPHYR_PLACEHOLDER_VECTOR repath_cache;
	ZCoordStruct location;
	
};

// original: Abs_Techno
struct ZephyrTechno {
	struct ZephyrObject_ parent;
	
	UNDEFINED_PTR TechnoType;
	UNDEFINED_PTR elements;
	ZPFCache pathfinding_cache;
	
};

#endif
