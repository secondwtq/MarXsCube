//
//  PathCube.cpp
//  MarXsCube
//
//  Created by SeconDatke on 2/19/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "Common.h"
#include "Path.h"
#include "Pathway.h"
#include "PathCube.h"

#include <vector>
#include <glm/glm.hpp>

Pathway path_from_gritpath(const std::vector<CubePoint> *src) {
	std::vector<glm::vec3> t;
	for (auto i = src->rbegin(); i != src->rend(); i++) {
		t.push_back(glm::vec3(i->x, i->y, 0)); }
	
	return Pathway(t.size(), t.data(), 256, false);
}