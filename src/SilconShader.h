//
//  SilconShader.h
//  MarXsCube
//
//  Created by SeconDatke on 2/11/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef __MarXsCube__SilconShader__
#define __MarXsCube__SilconShader__

#include "GLShaderExt.h"
#include "SilconCommon.h"

class SilconShader : public gl_shader_ext {
public:
	
	typedef SilconSpriteVertex type_vert_obj;
	
	void init_shader();
	void attribute_attr();
	void disable_attributes();
	
	DEF_ATTRIBUTE(position);
	DEF_ATTRIBUTE(texcoord);
	
	DEF_SAMPLER(texture_main);
};

#endif /* defined(__MarXsCube__SilconShader__) */
