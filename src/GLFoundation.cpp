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

const float SCALE_FACTOR = TransformScaleFactor;

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

void GLFoundation::view(float lkax, float lkay) {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
	gluLookAt(384, 384, 320, 0, 0, 0, 0, 0, 1);
	// 'gluLookAt' is deprecated: first deprecated in OS X 10.9 - "Use GLKMatrix4MakeLookAt" - WTF.
#pragma GCC diagnostic pop
	
	glTranslatef(lkay * SCALE_FACTOR, lkax * SCALE_FACTOR, 0);
	glScalef(SCALE_FACTOR, SCALE_FACTOR, SCALE_FACTOR);
}

GLuint vertex_arrays[5];

void init_opengl() {
	window_global->setActive();
	
	GLenum error = 0;
	error = glGetError();
	glGenVertexArrays(5, &vertex_arrays[0]);
	error = glGetError();
	glBindVertexArray(vertex_arrays[0]);
	error = glGetError();
	
//	glEnable(GL_DEPTH_TEST);
//	glDepthMask(GL_TRUE);
//	glClearDepth(1.f);
//	glDisable(GL_LIGHTING);
	
	GLsizei width = ATVBCube::setting<S::WindowSetting>().width,
		height = ATVBCube::setting<S::WindowSetting>().height;
	
	glViewport(0, 0, width, height);
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	glOrtho(0, width, height, 0, -16384.f, 16384.f);
//	glScalef(1, -1, 1);
	
}