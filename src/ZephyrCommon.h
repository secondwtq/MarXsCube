//
//  ZephyrCommon.h
//  MarXsCube
//
//  Created by SeconDatke on 2/10/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef MarXsCube_ZephyrCommon_h
#define MarXsCube_ZephyrCommon_h

typedef unsigned long ZSIZE_T;
typedef unsigned int GLIDX;

typedef void* UNDEFINED_PTR;

#include <stdbool.h>

#define ZEPHYR_VTABLE UNDEFINED_PTR _vptr

#define PHY_SCALE 0.1
#define PHY_SCALE_INV 10.0

#define CUBE_ENDLINE "\n"

#define ZEPHYR_PLACEHOLDER(len) struct { char _p[(len)]; }
#define ZEPHYR_PLACEHOLDER_VECTOR ZEPHYR_PLACEHOLDER(sizeof(UNDEFINED_PTR)+sizeof(UNDEFINED_PTR)+sizeof(UNDEFINED_PTR))

#define RTTIType_Abstract 0
#define RTTIType_Cell 1
#define RTTIType_Object 2
#define RTTIType_AbstractType 3
#define RTTIType_ObjectType 4
#define RTTIType_Anim 5
#define RTTIType_AnimType 6
#define RTTIType_Techno 7
#define RTTIType_TechnoType 8
#define RTTIType_TechnoRTTIIDTable 9

enum ZRenderLayerType_Type {
	LAYER_NONE = 0x0,
	LAYER_CELL = 0x1,
	LAYER_OBJECT = 0x2,
	LAYER_UI = 0x3
};

typedef struct {
	int x, y, z; } ZCoordStruct;

typedef struct {
	int x, y; } ZCubePoint;

typedef struct {
	UNDEFINED_PTR L;
	int m_ref;
} ZephyrScriptRef;

typedef struct {
	unsigned short degree;
} ZDirection;

typedef struct {
	ZSIZE_T idx;
	ZEPHYR_PLACEHOLDER_VECTOR cache;
} ZPFCache;

typedef struct {
	bool Enabled;
	bool Paused;
	bool TimerEnded;
	int current;
	int Interval;
	int LoopCount;
	int LoopTimed;
} ZCubeTimer;

#endif
