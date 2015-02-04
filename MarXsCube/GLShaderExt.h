//
//  GLShaderExt.h
//  MarXsCube
//
//  Created by SeconDatke on 2/4/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef __MarXsCube__GLShaderExt__
#define __MarXsCube__GLShaderExt__

#include "TilerRenderingBasic.h"
#include "GLShader.h"
#include <OpenGL/gltypes.h>

#define NAME_ATTRIBUTE(name) attr_##name
#define NAME_UNIFORM(name) uni_##name

#define _DEF_ATTRIBUTE(name) GLuint NAME_ATTRIBUTE(name)

#define _DEF_UNIFORM(name) GLuint NAME_UNIFORM(name)
#define _DEF_SAMPLER(name) _DEF_UNIFORM(name)

#define DEF_ATTRIBUTE(name) _DEF_ATTRIBUTE(name)
#define DEF_UNIFORM(name) _DEF_UNIFORM(name)
#define DEF_SAMPLER(name) _DEF_SAMPLER(name)

class gl_shader_ext : public gl_shader {
public:
	
	virtual void init_shader() { }
	
	virtual void attribute_attr() { }
	
	virtual void disable_attributes() { }
	
	void use_n_load() { this->use(); this->attribute_attr(); }
	
	virtual ~gl_shader_ext() { }
};

class tiler_shader : public gl_shader_ext {
public:
	
	typedef tiler_dataarray::VertObjectType type_vert_obj;
	
	void init_shader();
	
	void attribute_attr();
	
	void disable_attributes();
	
	DEF_ATTRIBUTE(position);
	DEF_ATTRIBUTE(normal);
	DEF_ATTRIBUTE(texcoord);
	DEF_ATTRIBUTE(blendweights);
	DEF_ATTRIBUTE(texindexes);
	
	DEF_SAMPLER(texture_main);
	DEF_SAMPLER(texture_second);
	DEF_SAMPLER(texture_heightfield);
	DEF_SAMPLER(texture_tileset);
	
};

#define BIND_TEXTURE(shader, name, texid, texunitid) (glActiveTexture(GL_TEXTURE##texunitid), glBindTexture(GL_TEXTURE_2D, (texid)), glUniform1i(shader.NAME_UNIFORM(name), (texunitid)))


#endif /* defined(__MarXsCube__GLShaderExt__) */
