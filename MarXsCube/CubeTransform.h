//
//  CubeTransform.h
//  MarXsCube
//
//  Created by SeconDatke on 1/25/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef __MarXsCube__CubeTransform__
#define __MarXsCube__CubeTransform__

#include "Common.h"
#include "SFML.h"

#include <glm/glm.hpp>

namespace CubeTransform {
	
	void generate_view_matrix(int ox, int oy);
	
	sf::Vector2f view_pos(const CoordStruct &coord);
	
	glm::vec3 look_at_vector();
	
}

#endif /* defined(__MarXsCube__CubeTransform__) */
