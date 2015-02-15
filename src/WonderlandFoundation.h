//
//  WonderlandFoundation.h
//  MarXsCube
//
//  Created by SeconDatke on 2/5/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef __MarXsCube__WonderlandFoundation__
#define __MarXsCube__WonderlandFoundation__

#include "Common.h"
#include "TeslaObject.h"

#include <cstddef>

namespace Wonderland {
	
	namespace Foundation {
		
		enum BlendDirection {
			LEFT, RIGHT, TOP, BOTTOM,
			LEFTTOP, RIGHTTOP, LEFTBOTTOM, RIGHTBOTTOM
		};
		
		void apply_blend(TeslaObject *chunk, std::size_t cell_idx, BlendDirection dir, bool overlay = false);
		
		void cell_fix_blend(TeslaObject *chunk, std::size_t cell_idx);
		
		void set_texture(TeslaObject *chunk, std::size_t index, const CoordStruct& position);
	
		void set_texture_and_blend(TeslaObject *chunk, std::size_t index, const CoordStruct& position);
		
		void buffer_update(TeslaObject *chunk);
		
		void blend_cells_batch(TeslaObject *chunk, const CoordStruct& position);
		
	}
	
}

#endif /* defined(__MarXsCube__WonderlandFoundation__) */
