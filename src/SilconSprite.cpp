//
//  SilconSprite.cpp
//  MarXsCube
//
//  Created by SeconDatke on 2/11/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "FSM.h"
#include "Generic.h"
#include "GLFoundation.h"
#include "ATVBCube.h"
#include "SilconShader.h"
#include "SilconSprite.h"

#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace FSMHelper;
using namespace ATVBCube::Helper;

namespace SilconSpriteGeneral {
	std::vector<SilconSpriteVertex> m_global_verts;
	gl_buffer<VBO, DYNAMIC> m_global_buffer;
	SilconShader m_shader;
	glm::mat4 m_mat_mvp;
}

extern GLuint vertex_arrays[5];

void SilconSpriteGeneral::init() {
	
	GLsizei width = ATVBCube::setting<S::WindowSetting>().width,
	height = ATVBCube::setting<S::WindowSetting>().height;
	
	glm::mat4 projection = glm::ortho(0.f, (float)width, (float)height, 0.f, -16384.f, 16384.f);
	m_mat_mvp = projection;
	
	m_global_buffer.init_with(m_global_verts.data(), 1000*6*sizeof(SilconSpriteVertex));
	push_empty_sprite();
	
	m_shader.load_file(SHADERTYPE::VERTEX, "shaders/silcon.vert");
	m_shader.load_file(SHADERTYPE::FRAG, "shaders/silcon.frag");
	m_shader.create();
	m_shader.init_shader();
	
	Generic::corelog() << "Silcon NT Shader Error Log: " << rn;
	std::cout << m_shader.log(VERTEX);
	std::cout << m_shader.log(FRAG);
	
}

void SilconSpriteGeneral::pre_render() {

//	glEnable(GL_DEPTH_TEST);
//	glDepthFunc(GL_LEQUAL);
//	glDepthMask(GL_TRUE);
//	GLFoundation::clear_depth();
//	glClearDepth(1.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	m_global_buffer.use();
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_global_verts.size() * sizeof(SilconSpriteVertex), m_global_verts.data());
	GLFoundation::unbind(m_global_buffer);
	
	m_global_buffer.use();
	m_shader.use_n_load();
	SET_UNIFORMAT4(SilconSpriteGeneral::m_shader, model_view_and_projection, SilconSpriteGeneral::m_mat_mvp);
}

void SilconSpriteGeneral::post_render() {
	GLFoundation::unbind(m_global_buffer);
	m_shader.disable_attributes();
}