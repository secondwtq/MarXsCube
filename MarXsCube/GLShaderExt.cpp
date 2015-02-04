//
//  GLShaderExt.cpp
//  MarXsCube
//
//  Created by SeconDatke on 2/4/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "GLShader.h"
#include "GLShaderExt.h"

#include <SFML/OpenGL.hpp>
#include <type_traits>

#undef DEF_ATTRIBUTE
#undef DEF_UNIFORM
#undef DEF_SAMPLER

#define DEF_ATTRIBUTE(name) ((this->NAME_ATTRIBUTE(name)) = (this->attribute(#name)))
#define DEF_UNIFORM(name) ((this->NAME_UNIFORM(name)) = (this->uniform(#name)))
#define DEF_SAMPLER(name) DEF_UNIFORM(name)

#define SET_ATTRIBUTE3(name, pos) (glVertexAttribPointer(this->NAME_ATTRIBUTE(name), 3, GL_FLOAT, GL_FALSE, sizeof(std::remove_reference<decltype(*this)>::type::type_vert_obj), (char *)(pos*sizeof(GLfloat))), glEnableVertexAttribArray(this->NAME_ATTRIBUTE(name)))

#define DISABLE_ATTRIBUTE(name) (glDisableVertexAttribArray(this->NAME_ATTRIBUTE(name)))

void tiler_shader::init_shader() {
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

void tiler_shader::attribute_attr() {
	
	SET_ATTRIBUTE3(position, 0);
	SET_ATTRIBUTE3(normal, 3);
	SET_ATTRIBUTE3(texcoord, 6);
	SET_ATTRIBUTE3(blendweights, 9);
	SET_ATTRIBUTE3(texindexes, 12);

}

void tiler_shader::disable_attributes() {
	
	DISABLE_ATTRIBUTE(position);
	DISABLE_ATTRIBUTE(normal);
	DISABLE_ATTRIBUTE(texcoord);
	DISABLE_ATTRIBUTE(blendweights);
	DISABLE_ATTRIBUTE(texindexes);
	
}