//
//  BulletDebugShader.h
//  MarXsCube
//
//  Created by SeconDatke on 2/14/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef MarXsCube_BulletDebugShader_h
#define MarXsCube_BulletDebugShader_h

#include "GLShaderExt.h"

#include <glm/glm.hpp>

struct BulletDebugVertex {
	glm::vec3 position;
	glm::vec3 color;
};

class BulletDebugShader : public gl_shader_ext {
public:
	
	typedef BulletDebugVertex type_vert_obj;
	
	void init_shader();
	void attribute_attr();
	void disable_attributes();
	
	DEF_ATTRIBUTE(position);
	DEF_ATTRIBUTE(color);
	
	DEF_UNIFORM(model_view_and_projection);
	
};

#endif
