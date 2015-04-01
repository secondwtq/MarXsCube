//
//  WonderlandBrush.cpp
//  MarXsCube
//
//  Created by SeconDatke on 2/23/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "Common.h"
#include "SFML.h"
#include "SilconThread.h"
#include "TeslaObject.h"
#include "GLFoundation.h"
#include "WonderlandBrushShader.h"
#include "WonderlandBrush.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

WonderlandBrushShader shader_test;

static const GLfloat g_quad_vertex_buffer_data[] = {
	-1.0f, -1.0f, 0.0f, 0.f, 0.f, 0.0f,
	1.0f, -1.0f, 0.0f, 1.f, 0.f, 0.0f,
	-1.0f,  1.0f, 0.0f, 0.f, 1.f, 0.0f,
	-1.0f,  1.0f, 0.0f, 0.f, 1.f, 0.0f,
	1.0f, -1.0f, 0.0f, 1.f, 0.f, 0.0f,
	1.0f,  1.0f, 0.0f, 1.f, 1.f, 0.0f,
};

namespace {
	GLenum _evan_fb_bufs[1] = { GL_COLOR_ATTACHMENT0 };
}

bool EvanDrawBuffer::bind(GLIDX target) {
	glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer_id);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, target, 0);
	this->m_target_id = target;
	glDrawBuffers(1, _evan_fb_bufs);
	glViewport(0, 0, static_cast<int>(this->m_width), static_cast<int>(this->m_height));

	this->m_buf_paint.use();
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		printf("Framebuffer not complete!\n");
		return false;
	}
	return true;
}

void EvanDrawBuffer::unbind() {
	GLFoundation::unbind(this->m_buf_paint);
	GLFoundation::reset_target();
}

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

void BrushTest(TeslaObject *chunk, EvanDrawBuffer *buffer, TextureAtlas *brush, TextureAtlas *fill, CubePoint& center, float radius) {

	Acheron::Silcon.pause();

	GLFoundation::enable_blend_for([chunk, buffer, brush, fill, &center, radius] () {

		glm::vec2 scale { 1, 1 }, position { center.x, center.y };
		scale *= radius;
		glm::mat4 projection = glm::ortho(0.f, (float)2048, (float)2048, 0.f, -16384.f, 16384.f);

		buffer->bind(chunk->get_texid_bg());
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data);
		shader_test.use_n_load();
		SET_UNIFORMAT4(shader_test, model_view_and_projection, projection);
		SET_UNIFORM2(shader_test, brush_position, position);
		SET_UNIFORM2(shader_test, brush_scale, scale);
		BIND_TEXTURE(shader_test, texture_mask, brush->texture.m_texture, 0);
		BIND_TEXTURE(shader_test, texture_fill, fill->texture.m_texture, 1);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		shader_test.disable_attributes();
		buffer->unbind();

	});

	Acheron::Silcon.invoke();
}
