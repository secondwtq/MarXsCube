//
//  PathUtils.h
//  MarXsCube
//
//  Created by SeconDatke on 2/19/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef MarXsCube_PathUtils_h
#define MarXsCube_PathUtils_h

#include <limits>
#include <cmath>

template<typename T>
bool zero(T const& v, T const& tolerance = std::numeric_limits<T>::epsilon()) {
	return abs(v) <= tolerance; }

template<typename T>
T clamp(T const& valueToClamp, T const& minValue, T const& maxValue) {
	assert(minValue <= maxValue && "minValue must be lesser or equal to maxValue.");
	
	if (valueToClamp < minValue) return minValue;
	else if (valueToClamp > maxValue) return maxValue;
	
	return valueToClamp;
}

inline float modulo(float x, float y) {
	assert(0.0f != y && "Division by zero.");
	return std::fmod(x, y); }

template<typename T>
void shrink_to_fit(std::vector<T>& v) {
	std::vector<T>(v).swap(v); }

#endif
