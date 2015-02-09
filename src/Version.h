//
//  Version.h
//  MarXsCube
//
//  Created by SeconDatke on 2/7/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef MarXsCube_Version_h
#define MarXsCube_Version_h

#include "Platform.h"

#define CUBE_ENGINE_NAME "Mar?sCube"

#ifdef CUBE_PLATFORM_DARWIN
#undef CUBE_ENGINE_NAME
#define CUBE_ENGINE_NAME "MarXsCube"
#elif defined(CUBE_PLATFORM_WINDOWS)
#undef CUBE_ENGINE_NAME
#define CUBE_ENGINE_NAME "MarKsCube"
#endif

#define CUBE_STAGE_NAME "Prototype"

#define CUBE_FULL_NAME CUBE_ENGINE_NAME " (" CUBE_STAGE_NAME ")"

#endif /* defined(MarXsCube_Version_h) */
