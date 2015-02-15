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
	
		void set_texture_and_blend(TeslaObject *chunk, std::size_t index, const CoordStruct& position);
		
		void buffer_update(TeslaObject *chunk);
		
	}
	
}

#endif /* defined(__MarXsCube__WonderlandFoundation__) */
