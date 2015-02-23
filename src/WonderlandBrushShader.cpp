//
//  WonderlandBrushShader.cpp
//  MarXsCube
//
//  Created by SeconDatke on 2/23/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "WonderlandBrush.h"
#include "WonderlandBrushShader.h"

#include "GLShaderExtDef.h"

void WonderlandBrushShader::init_shader() {
	
	DEF_ATTRIBUTE_NT(position);
	DEF_ATTRIBUTE_NT(texcoord);
	DEF_UNIFORM(model_view_and_projection);

}

void WonderlandBrushShader::attribute_attr() {
	
	SET_ATTRIBUTE3_NT(position, 0, 0);
	SET_ATTRIBUTE3_NT(texcoord, 1, 3);
	
}

void WonderlandBrushShader::disable_attributes() {
	
	DISABLE_ATTRIBUTE_NT(position, 0);
	DISABLE_ATTRIBUTE_NT(texcoord, 1);
	
}