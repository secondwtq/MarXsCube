//
//  PathCube.h
//  MarXsCube
//
//  Created by SeconDatke on 2/19/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef __MarXsCube__PathCube__
#define __MarXsCube__PathCube__

#include "Common.h"
#include "Path.h"
#include "Pathway.h"
#include <vector>

Pathway path_from_gritpath(const std::vector<CubePoint> *src);

#endif /* defined(__MarXsCube__PathCube__) */
