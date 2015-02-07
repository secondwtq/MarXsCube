//
//  Timer.cpp
//  MarXsCube
//
//  Created by SeconDatke on 2/7/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "Platform.h"
#include "Timer.h"

#include <cstdint>

namespace {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
	uint64_t rate = -1; // not guaranteed to be available
	double rate_d = -1.;
#pragma GCC diagnostic pop
}

#ifdef CUBE_PLATFORM_DARWIN
#include <mach/mach_time.h>
#elif defined(CUBE_PLATFORM_WINDOWS)
#include <windows.h>
#elif defined(CUBE_PLATFROM_POSIX)
#include <time.h>
#endif

void timer_init() {

#ifdef CUBE_PLATFORM_DARWIN
	mach_timebase_info_data_t rate_mach;
	mach_timebase_info(&rate_mach);
	rate_d = 1000000. * (rate_mach.numer / (double)rate_mach.denom);
#elif defined(CUBE_PLATFROM_WINDOWS)
	LARGE_INTEGER f;
 
	QueryPerformanceFrequency(&f);
	rate = f.QuadPart;
	rate_d = (double)rate;
#elif defined(CUBE_PLATFORM_POSIX)
	
#endif
	
//	rate_d = (double)rate;
}

#include <stdio.h>

double time_in_millisec() {
	uint64_t now = -1;
	
#ifdef CUBE_PLATFORM_DARWIN
	now = mach_absolute_time();
#elif defined(CUBE_PLATFORM_WINDOWS)
	LARGE_INTEGER f;
	QueryPerformanceFrequency(&f);
	now = f.QuadPart;
#elif defined(CUBE_PLATFORM_POSIX)
	
#endif
	
	return (now / rate_d);
}
