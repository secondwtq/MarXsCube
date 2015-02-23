//
//  WonderlandBrush.cpp
//  MarXsCube
//
//  Created by SeconDatke on 2/23/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "SilconThread.h"
#include "TeslaObject.h"
#include "GLFoundation.h"
#include "WonderlandBrushShader.h"
#include "WonderlandBrush.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

WonderlandBrushShader shader_test;

static const GLfloat g_quad_vertex_buffer_data[] = {
	-128.0f, -128.0f, 0.0f, -1.0f, -1.0f, 0.0f,
	128.0f, -128.0f, 0.0f, -1.0f, -1.0f, 0.0f,
	-128.0f,  128.0f, 0.0f, -1.0f, -1.0f, 0.0f,
	-128.0f,  128.0f, 0.0f, -1.0f, -1.0f, 0.0f,
	128.0f, -128.0f, 0.0f, -1.0f, -1.0f, 0.0f,
	128.0f,  128.0f, 0.0f, -1.0f, -1.0f, 0.0f,
};

void BrushInit() {
	Acheron::Silcon.pause();
	shader_test.load_file(SHADERTYPE::VERTEX, "shaders/wonderland_brush.vert");
	shader_test.load_file(SHADERTYPE::FRAG, "shaders/wonderland_brush.frag");
	shader_test.create();
	shader_test.init_shader();
	
	std::cout << shader_test.log(VERTEX);
	std::cout << shader_test.log(FRAG);
	Acheron::Silcon.invoke();
}

void BrushTest(TeslaObject *chunk) {
	Acheron::Silcon.pause();
	GLenum e = 0;
	
	glBindFramebuffer(GL_FRAMEBUFFER, chunk->m_fb_bgpaint);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, chunk->m_tex_background, 0);
	GLenum bufs[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, bufs);
	glViewport(0, 0, 2048, 2048);
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		printf("Framebuffer not complete!\n");
	
	glm::mat4 projection = glm::ortho(0.f, (float)2048, (float)2048, 0.f, -16384.f, 16384.f);
	
	chunk->m_buf_bgpaint->use();
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data);
	shader_test.use_n_load();
	SET_UNIFORMAT4(shader_test, model_view_and_projection, projection);
	
	glDrawArrays(GL_TRIANGLES, 0, 6);
	
	
	GLFoundation::unbind(*chunk->m_buf_bgpaint);
	shader_test.disable_attributes();
	GLFoundation::reset_target();
	
	Acheron::Silcon.invoke();
}