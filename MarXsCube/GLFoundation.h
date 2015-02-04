//
//  GLFoundation.h
//  MarXsCube
//
//  Created by SeconDatke on 1/25/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef __MarXsCube__GLFoundation__
#define __MarXsCube__GLFoundation__

void init_opengl();

void render_gl();

void raise_verts();

void tiler_array_test();

class GLFoundation {
	public:
	
		static void unbind_shader();
		static void view(float lkax, float lkay);
};

#endif /* defined(__MarXsCube__GLFoundation__) */
