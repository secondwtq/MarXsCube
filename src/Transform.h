#ifndef CUBE_E_TRANSFORM_H
#define CUBE_E_TRANSFORM_H

#include "Common.h"

namespace obsTransform {
	
	void UpdateVm(int ox = 0, int oy = 0);

	CoordStruct GetWorldPos(const CubePoint &pt);
	
}

#endif