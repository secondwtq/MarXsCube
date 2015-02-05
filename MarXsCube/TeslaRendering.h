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

class TeslaRenderingManger {
public:
	std::vector<TeslaObject *> chunks;
	
	void add_chunk(TeslaObject *src) {
		this->chunks.push_back(src); }
	
	void Render();
};

#endif /* defined(__MarXsCube__TeslaRendering__) */
