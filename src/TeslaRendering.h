//
//  TeslaRendering.h
//  MarXsCube
//
//  Created by SeconDatke on 2/5/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef __MarXsCube__TeslaRendering__
#define __MarXsCube__TeslaRendering__

#include "TeslaObject.h"

#include <vector>
#include <glm/glm.hpp>

class TeslaRenderingManger {
public:
	std::vector<TeslaObject *> chunks;
	
	void add_chunk(TeslaObject *src) {
		this->chunks.push_back(src); }
	
	void pre_render();
	void Render();
	
	glm::mat4 m_mat_mvp;
};

#endif /* defined(__MarXsCube__TeslaRendering__) */
