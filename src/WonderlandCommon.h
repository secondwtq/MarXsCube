//
//  WonderlandCommon.h
//  MarXsCube
//
//  Created by SeconDatke on 2/22/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef __MarXsCube__WonderlandCommon__
#define __MarXsCube__WonderlandCommon__

#include <cstddef>

namespace Wonderland {
	
	namespace Foundation {

enum BlendDirection {
	LEFT, RIGHT, TOP, BOTTOM,
	LEFTTOP, RIGHTTOP, LEFTBOTTOM, RIGHTBOTTOM
};
		
struct wonderland_chunk_header {
	const char HEADER_ID[16] = "!MXCUBE_CBSB!";
	std::size_t num_cells = 0;
	std::size_t num_verts = 0;
	std::size_t num_idxes = 0;
};
		
	}
}

#endif /* defined(__MarXsCube__WonderlandCommon__) */
