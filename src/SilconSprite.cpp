//
//  SilconSprite.cpp
//  MarXsCube
//
//  Created by SeconDatke on 2/11/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "GLFoundation.h"
#include "ATVBCube.h"
#include "SilconShader.h"
#include "SilconSprite.h"

#include <vector>
#include <iostream>

using namespace ATVBCube::Helper;

namespace SilconSpriteGeneral {
	std::vector<SilconSpriteVertex> m_global_verts;
	gl_buffer<VBO, DYNAMIC> m_global_buffer;
	SilconShader m_shader;
}

void SilconSpriteGeneral::init() {
	
	m_global_buffer.init_with(m_global_verts.data(), 1000*6*sizeof(SilconSpriteVertex));
	push_empty_sprite();
	
	m_shader.load_file(SHADERTYPE::VERTEX, "silcon.vert");
	m_shader.load_file(SHADERTYPE::FRAG, "silcon.frag");
	m_shader.create();
	m_shader.init_shader();
	
	printf("Error log:\n");
	std::cout << m_shader.log(VERTEX);
	std::cout << m_shader.log(FRAG);
}

void SilconSpriteGeneral::pre_render() {
	GLsizei width = ATVBCube::setting<S::WindowSetting>().width,
	height = ATVBCube::setting<S::WindowSetting>().height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, height, 0, -16384.f, 16384.f);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	GLFoundation::clear_depth();
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	
	m_global_buffer.use();
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_global_verts.size() * sizeof(SilconSpriteVertex), m_global_verts.data());
	GLFoundation::unbind(m_global_buffer);
	
	m_global_buffer.use();
	m_shader.use_n_load();
}

void SilconSpriteGeneral::post_render() {
	GLFoundation::unbind(m_global_buffer);
	m_shader.disable_attributes();
}

void SilconSprite::render() {
	
	SET_UNIFORM2(SilconSpriteGeneral::m_shader, sprite_position, this->position);
	SET_UNIFORM4(SilconSpriteGeneral::m_shader, color_multiply, this->color_multiply);
	BIND_TEXTURE(SilconSpriteGeneral::m_shader, texture_main, this->m_texture_id, 0);
	glDrawArrays(GL_TRIANGLES, static_cast<GLint>(6*this->m_sprite_id), 6);
	
}