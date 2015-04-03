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

#ifdef __APPLE__
#define CUBE_PLATFORM_OS_X
#define CUBE_PLATFORM_DARWIN
#define CUBE_PLATFORM_POSIX
#define CUBE_PLATFORM_MACH
#elif defined(_WIN32)
#define CUBE_PLATFORM_WINDOWS
#define CUBE_PLATFORM_NONPOSIX
#if defined(_MSC_VER)
#undef CUBE_COMPILER_UNKNOWN
#define CUBE_COMPILER_MSVC
#pragma message "MSVC is not supported by MarKsCube yet."
#endif
#elif defined(__linux)
#define CUBE_PLATFORM_LINUX
#define CUBE_PLATFORM_POSIX
#else
#pragma GCC error "What platform are you targeting?"
#endif

#if defined(__GNUC__)
	#undef CUBE_COMPILER_UNKNOWN
	#define CUBE_COMPILER_GCC_BASED

		#if defined(__clang__)
			#define CUBE_COMPILER_CLANG

		#else
			#define CUBE_COMPILER_GCC

			#if defined(__MINGW32__)
				#define CUBE_COMPILER_MINGW
				#define CUBE_COMPILER_MINGW32
			#endif

	#endif
#endif

// well gcc ignored my ignore diagnostic -Wunused-variable
// clang is a good child, at least in this term
// so ...
// http://stackoverflow.com/questions/9832687/strange-diagnostic-pragma-behavior-in-gcc-4-6
// http://stackoverflow.com/questions/8071579/hide-gcc-warning-set-but-not-used
#ifdef CUBE_COMPILER_GCC_BASED
#define CUBEMUTEW_NOT_USED __attribute__ ((unused))
#else
#define CUBEMUTEW_NOT_USED
#endif

#endif /* defined(__MarXsCube__Platform__) */
