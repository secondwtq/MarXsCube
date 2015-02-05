//
//  TilerRendering.cpp
//  MarXsCube
//
//  Created by SeconDatke on 2/5/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "GLFoundation.h"
#include "TilerObject.h"
#include "TilerRendering.h"

void TilerRenderingManger::Render() {
	glm::vec3 look_at_vec = CubeTransform::look_at_vector();
	GLFoundation::clear_depth();
	GLFoundation::view(look_at_vec.x, look_at_vec.y);
	
	for (auto chunk : this->chunks) {
		chunk->Render(); }
	
	GLFoundation::view(look_at_vec.x, look_at_vec.y);
}