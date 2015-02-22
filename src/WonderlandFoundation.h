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
#include "WonderlandCommon.h"

#include <cstddef>

namespace Wonderland {
	
	namespace Foundation {
		
		void apply_blend(TeslaObject *chunk, std::size_t cell_idx, BlendDirection dir, std::size_t tile_id, bool overlay = false);
		
		void cell_fix_blend(TeslaObject *chunk, std::size_t cell_idx);
		
		void set_texture(TeslaObject *chunk, std::size_t index, const CoordStruct& position);
	
		void set_texture_and_blend(TeslaObject *chunk, std::size_t index, const CoordStruct& position);
		
		void buffer_update(TeslaObject *chunk);
		
		void blend_cells_batch(TeslaObject *chunk, const CoordStruct& position);
		
		std::string serialize_chunk(TeslaObject *chunk);
		
		tesla_dataarray *deserialize_chunk(const std::string& src, tesla_dataarray *dest);
		
	}
	
}

#endif /* defined(__MarXsCube__WonderlandFoundation__) */
