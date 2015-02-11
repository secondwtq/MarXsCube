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
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace ATVBCube::Helper;

namespace SilconSpriteGeneral {
	std::vector<SilconSpriteVertex> m_global_verts;
	gl_buffer<VBO, DYNAMIC> m_global_buffer;
	SilconShader m_shader;
	glm::mat4 m_mat_mvp;
}

GLfloat vertices[] = {	   0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f,
	0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f,
	-0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, };

GLuint testvert_buf = 0;

extern GLuint vertex_arrays[5];

void SilconSpriteGeneral::init() {
	
	GLsizei width = ATVBCube::setting<S::WindowSetting>().width,
	height = ATVBCube::setting<S::WindowSetting>().height;
	
	glm::mat4 projection = glm::ortho(0.f, (float)width, (float)height, 0.f, -16384.f, 16384.f);
	m_mat_mvp = projection;
	
	m_global_buffer.init_with(m_global_verts.data(), 1000*6*sizeof(SilconSpriteVertex));
	push_empty_sprite();
	
	m_shader.load_file(SHADERTYPE::VERTEX, "silcon.vert");
	m_shader.load_file(SHADERTYPE::FRAG, "silcon.frag");
	m_shader.create();
	m_shader.init_shader();
	
	printf("Error log:\n");
	std::cout << m_shader.log(VERTEX);
	std::cout << m_shader.log(FRAG);
	
	glGenBuffers(1, &testvert_buf);
	glBindBuffer(GL_ARRAY_BUFFER, testvert_buf);
	glBufferData(GL_ARRAY_BUFFER, 3*6*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
}

void SilconSpriteGeneral::pre_render() {
	
//	glViewport(0, 0, width, height);
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	glOrtho(0, width, height, 0, -16384.f, 16384.f);
	
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
	
	GLFoundation::clear_depth();
//	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	
	m_global_buffer.use();
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_global_verts.size() * sizeof(SilconSpriteVertex), m_global_verts.data());
	GLFoundation::unbind(m_global_buffer);
	
	glBindBuffer(GL_ARRAY_BUFFER, testvert_buf);
	m_shader.use_n_load();
	glBindVertexArrayAPPLE(vertex_arrays[0]);
	SET_UNIFORMAT4(m_shader, model_view_and_projection, m_mat_mvp);
	SET_UNIFORM2(SilconSpriteGeneral::m_shader, sprite_position, glm::vec2(0, 0));
	glDrawArrays(GL_TRIANGLES, 0, 3);
	
	m_global_buffer.use();
	m_shader.use_n_load();
}

void SilconSpriteGeneral::post_render() {
	GLFoundation::unbind(m_global_buffer);
	m_shader.disable_attributes();
}

void SilconSprite::render() {
	
	SilconSpriteGeneral::m_shader.use_n_load();
	SilconSpriteGeneral::m_global_buffer.use();
	SilconSpriteGeneral::m_shader.attribute_attr();
	
	SET_UNIFORMAT4(SilconSpriteGeneral::m_shader, model_view_and_projection, SilconSpriteGeneral::m_mat_mvp);
	SET_UNIFORM2(SilconSpriteGeneral::m_shader, sprite_position, this->position);
//	SET_UNIFORM4(SilconSpriteGeneral::m_shader, color_multiply, this->color_multiply);
	BIND_TEXTURE(SilconSpriteGeneral::m_shader, texture_main, this->m_texture_id, 0);
	glDrawArrays(GL_TRIANGLES, static_cast<GLint>(6*this->m_sprite_id), 6);
	
}