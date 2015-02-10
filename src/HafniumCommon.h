//
//  LuaJit_Bridge.h
//  MarXsCube
//
//  Created by SeconDatke on 12/14/14.
//  Copyright (c) 2014 MarXsCube Staff. All rights reserved.
//

#ifndef __MarXsCube__HafniumCommon__
#define __MarXsCube__HafniumCommon__

#include "buildconf.h"

#ifdef CUBE_CONFIG_ENABLE_TERRA

#include <terra.h>
#define hafnium_dofile(L,filename) (terra_dofile((L), (filename)))

#else

#include <luajit-2.0/lua.hpp>
#define hafnium_dofile(L,filename) (luaL_dofile((L), (filename)))

#endif

#include <LuaBridge/LuaBridge.h>

#endif
