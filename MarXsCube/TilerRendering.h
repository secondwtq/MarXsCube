//
//  TilerRendering.h
//  MarXsCube
//
//  Created by SeconDatke on 2/5/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef __MarXsCube__TilerRendering__
#define __MarXsCube__TilerRendering__

#include "TilerObject.h"

#include <vector>

class TilerRenderingManger {
public:
	std::vector<TilerObject *> chunks;
	
	void add_chunk(TilerObject *src) {
		this->chunks.push_back(src); }
	
	void Render();
};

#endif /* defined(__MarXsCube__TilerRendering__) */
