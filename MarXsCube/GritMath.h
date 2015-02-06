//
//  GritMath.h
//  MarXsCube
//
//  Created by SeconDatke on 2/6/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef __MarXsCube__GritMath__
#define __MarXsCube__GritMath__

#include <cmath>

template <typename T>
inline float gmag2(const T& a) {
	return std::sqrt(std::pow(a.x, 2) + std::pow(a.y, 2)); }

template <typename T>
inline T gsub2(const T& a, const T& b) {
	return T(a.x-b.x, a.y-b.y); }

#endif /* defined(__MarXsCube__GritMath__) */