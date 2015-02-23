//
//  WonderlandBrushShader.h
//  MarXsCube
//
//  Created by SeconDatke on 2/23/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef __MarXsCube__WonderlandBrushShader__
#define __MarXsCube__WonderlandBrushShader__

#include "WonderlandBrush.h"
#include "GLShaderExt.h"

class WonderlandBrushShader : public gl_shader_ext {
public:
	
	typedef WonderlandBrushVertex type_vert_obj;
	
	void init_shader();
	void attribute_attr();
	void disable_attributes();
	
	DEF_ATTRIBUTE(position);
	DEF_ATTRIBUTE(texcoord);
	
	DEF_UNIFORM(model_view_and_projection);
	
};

#endif /* defined(__MarXsCube__WonderlandBrushShader__) */
