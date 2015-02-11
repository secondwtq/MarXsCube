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
	DEF_ATTRIBUTE(position);
	DEF_ATTRIBUTE(normal);
	DEF_ATTRIBUTE(texcoord);
	DEF_ATTRIBUTE(blendweights);
	DEF_ATTRIBUTE(texindexes);
	
	DEF_SAMPLER(texture_main);
	DEF_SAMPLER(texture_second);
	DEF_SAMPLER(texture_heightfield);
	DEF_SAMPLER(texture_tileset);
}

void tesla_shader::attribute_attr() {
	
	SET_ATTRIBUTE3(position, 0);
	SET_ATTRIBUTE3(normal, 3);
	SET_ATTRIBUTE3(texcoord, 6);
	SET_ATTRIBUTE3(blendweights, 9);
	SET_ATTRIBUTE3(texindexes, 12);

}

void tesla_shader::disable_attributes() {
	
	DISABLE_ATTRIBUTE(position);
	DISABLE_ATTRIBUTE(normal);
	DISABLE_ATTRIBUTE(texcoord);
	DISABLE_ATTRIBUTE(blendweights);
	DISABLE_ATTRIBUTE(texindexes);
	
}