//
//  ATVBCube.h
//  MarXsCube
//
//  Created by SeconDatke on 2/2/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef __MarXsCube__ATVBCube__
#define __MarXsCube__ATVBCube__

#include "SFML.h"

#include <string>

namespace ATVBCube {
	
	struct CubeInit_WindowSetting {
	public:
		sf::ContextSettings context;
		
		std::string window_title;
		
		unsigned int width, height;
		
		unsigned int fps_limit;
		bool enable_vsync;
	};
	
	void read_context_setting();
	
	CubeInit_WindowSetting& get_window_setting();
	
}

#endif /* defined(__MarXsCube__ATVBCube__) */
