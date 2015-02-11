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
	DEF_ATTRIBUTE_NT(position);
	DEF_ATTRIBUTE_NT(texcoord);
	
	DEF_UNIFORM(model_view_and_projection);
	DEF_UNIFORM(sprite_position);
	DEF_UNIFORM(color_multiply);
	DEF_SAMPLER(texture_main);
}

void SilconShader::attribute_attr() {
	
	SET_ATTRIBUTE3_NT(position, 0, 0);
	SET_ATTRIBUTE3_NT(texcoord, 1, 3);
	
}

void SilconShader::disable_attributes() {
	
	DISABLE_ATTRIBUTE_NT(position, 0);
	DISABLE_ATTRIBUTE_NT(texcoord, 1);
	
}
