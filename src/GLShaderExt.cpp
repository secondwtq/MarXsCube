//
//  GLShaderExt.cpp
//  MarXsCube
//
//  Created by SeconDatke on 2/4/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "GLShader.h"
#include "GLShaderExt.h"
#include "GLShaderExtDef.h"

void tesla_shader::init_shader() {
	DEF_ATTRIBUTE_NT(position);
	DEF_ATTRIBUTE_NT(normal);
	DEF_ATTRIBUTE_NT(texcoord);
	DEF_ATTRIBUTE_NT(blendweights);
	DEF_ATTRIBUTE_NT(texindexes);
	
	DEF_UNIFORM(model_view_and_projection);
	DEF_UNIFORM(chunk_position);
	DEF_SAMPLER(texture_main);
	DEF_SAMPLER(texture_second);
	DEF_SAMPLER(texture_heightfield);
	DEF_SAMPLER(texture_tileset);
}

void tesla_shader::attribute_attr() {
	
	SET_ATTRIBUTE3_NT(position, 0, 0);
	SET_ATTRIBUTE3_NT(normal, 1, 3);
	SET_ATTRIBUTE3_NT(texcoord, 2, 6);
	SET_ATTRIBUTE3_NT(blendweights, 3, 9);
	SET_ATTRIBUTE3_NT(texindexes, 4, 12);

}

void tesla_shader::disable_attributes() {
	
	DISABLE_ATTRIBUTE(position);
	DISABLE_ATTRIBUTE(normal);
	DISABLE_ATTRIBUTE(texcoord);
	DISABLE_ATTRIBUTE(blendweights);
	DISABLE_ATTRIBUTE(texindexes);
	
}