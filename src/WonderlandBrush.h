//
//  WonderlandBrush.h
//  MarXsCube
//
//  Created by SeconDatke on 2/23/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef __MarXsCube__WonderlandBrush__
#define __MarXsCube__WonderlandBrush__

#include "TeslaObject.h"

#include <glm/glm.hpp>

struct WonderlandBrushVertex {
	glm::vec3 position;
	glm::vec3 texcoord;
};

void BrushInit();

void BrushTest(TeslaObject *chunk);

#endif /* defined(__MarXsCube__WonderlandBrush__) */
