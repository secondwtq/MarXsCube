//
//  SilconShader.cpp
//  MarXsCube
//
//  Created by SeconDatke on 2/11/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "GLShaderExt.h"
#include "SilconShader.h"

#include "GLShaderExtDef.h"

void SilconShader::init_shader() {
	DEF_ATTRIBUTE(position);
	DEF_ATTRIBUTE(texcoord);
	
	DEF_UNIFORM(model_view_and_projection);
	DEF_UNIFORM(sprite_position);
	DEF_UNIFORM(color_multiply);
	DEF_SAMPLER(texture_main);
}

void SilconShader::attribute_attr() {
	
	SET_ATTRIBUTE3(position, 0);
	SET_ATTRIBUTE3(texcoord, 3);
	
}

void SilconShader::disable_attributes() {
	
	DISABLE_ATTRIBUTE(position);
	DISABLE_ATTRIBUTE(texcoord);
	
}
