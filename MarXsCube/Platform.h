//
//  Platform.h
//  MarXsCube
//
//  Created by SeconDatke on 2/7/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef __MarXsCube__Platform__
#define __MarXsCube__Platform__

#define CUBE_COMPILER_UNKNOWN

#ifdef _WIN32
#define CUBE_PLATFORM_WINDOWS
#define CUBE_PLATFORM_NONPOSIX
#endif

#ifdef __APPLE__
#define CUBE_PLATFORM_OS_X
#define CUBE_PLATFORM_DARWIN
#define CUBE_PLATFORM_POSIX
#define CUBE_PLATFORM_MACH
#elif defined(_MSC_VER)
#define CUBE_PLATFORM_WINDOWS
#define CUBE_PLATFORM_NONPOSIX
#undef CUBE_COMPILER_UNKNOWN
#define CUBE_COMPILER_MSVC
#pragma message "MSVC is not supported by MarKsCube yet."
#else
#pragma GCC error "What platform are you targeting?"
#endif

#endif /* defined(__MarXsCube__Platform__) */
