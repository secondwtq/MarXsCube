//
//  WonderlandBrush.h
//  MarXsCube
//
//  Created by SeconDatke on 2/23/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef __MarXsCube__WonderlandBrush__
#define __MarXsCube__WonderlandBrush__

#include "GLFoundation.h"
#include "TeslaObject.h"

#include <vector>
#include <glm/glm.hpp>

struct EvanBrushVertex {
	glm::vec3 position;
	glm::vec3 texcoord;
};

struct EvanDrawBuffer {
	std::size_t m_width, m_height;
	
	std::vector<EvanBrushVertex> buffer_data;
	GLIDX framebuffer_id = 0;
	GLIDX m_target_id = 0;
	gl_buffer<VBO, DYNAMIC> m_buf_paint;
	
	EvanDrawBuffer (std::size_t width, std::size_t height) :
		m_width(width), m_height(height) {
		this->m_buf_paint.init_with(this->buffer_data.data(), 16 * sizeof(EvanBrushVertex));
		glGenFramebuffers(1, &this->framebuffer_id);
	}
	
	bool bind(GLIDX target);
	
	void unbind();
};

void BrushInit();

void BrushTest(TeslaObject *chunk, EvanDrawBuffer *buffer, TextureAtlas *brush, TextureAtlas *fill, CubePoint& center, float radius);

#endif /* defined(__MarXsCube__WonderlandBrush__) */
