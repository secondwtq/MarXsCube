//
//  GLFoundation.cpp
//  MarXsCube
//
//  Created by SeconDatke on 1/25/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "Common.h"
#include "SFML.h"

#include "GLFoundation.h"
#include "ATVBCube.h"

#include "cubegl.h"
#include <iostream>

extern sf::RenderWindow *window_global;

using namespace ATVBCube::Helper;

template <SILCON_GLBUFFER_TYPE BufferT, SILCON_GLBUFFER_USAGE UsageT>
void gl_buffer<BufferT, UsageT>::init_with(void *data, std::size_t size) {
	glGenBuffers(1, &(this->m_buffer_id));
	glBindBuffer(BufferT, this->m_buffer_id);
	glBufferData(BufferT, size, data, UsageT);
	glBindBuffer(BufferT, 0);
}

template <SILCON_GLBUFFER_TYPE BufferT, SILCON_GLBUFFER_USAGE UsageT>
void gl_buffer<BufferT, UsageT>::use() {
	glBindBuffer(BufferT, this->m_buffer_id); }

template void gl_buffer<VBO, DYNAMIC>::use();
template void gl_buffer<IBO, DYNAMIC>::use();

template void gl_buffer<VBO, DYNAMIC>::init_with(void *data, std::size_t size);
template void gl_buffer<IBO, DYNAMIC>::init_with(void *data, std::size_t size);

void GLFoundation::unbind_shader() {
	return glUseProgram(0); }

void GLFoundation::clear_depth() {
	return glClear(GL_DEPTH_BUFFER_BIT); }

GLuint vertex_arrays[5];

void init_opengl() {
	window_global->setActive();

	glGenVertexArrays(5, &vertex_arrays[0]);
	glBindVertexArray(vertex_arrays[0]);
	
//	glEnable(GL_DEPTH_TEST);
//	glDepthMask(GL_TRUE);
	glClearDepth(1.f);
	
	GLsizei width = ATVBCube::setting<S::WindowSetting>().width,
		height = ATVBCube::setting<S::WindowSetting>().height;
	
	glViewport(0, 0, width, height);
	
}