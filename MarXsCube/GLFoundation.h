//
//  GLFoundation.h
//  MarXsCube
//
//  Created by SeconDatke on 1/25/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef __MarXsCube__GLFoundation__
#define __MarXsCube__GLFoundation__

#include "Common.h"
#include <SFML/OpenGL.hpp>
#include <vector>

void init_opengl();

void render_gl();

void raise_verts();

void tiler_array_test();

class GLFoundation {
	public:
	
		static void unbind_shader();
		static void view(float lkax, float lkay);
		static void clear_depth();
	
		template<typename BufferT>
		static void unbind(const BufferT& src) {
			return glBindBuffer(src.buffer_type, 0); }
	
};

enum SILCON_GLBUFFER_TYPE {
	VBO = GL_ARRAY_BUFFER,
	IBO = GL_ELEMENT_ARRAY_BUFFER
};

enum SILCON_GLBUFFER_USAGE {
	STATIC = GL_STATIC_DRAW,
	DYNAMIC = GL_DYNAMIC_DRAW
};

template <SILCON_GLBUFFER_TYPE BufferT, SILCON_GLBUFFER_USAGE UsageT>
class gl_buffer {
public:
	
	void init_with(void *data, std::size_t size);
	
	template <typename T>
	void init_with(const std::vector<T> &data) {
		this->init_with(reinterpret_cast<void *>(const_cast<T *>(data.data())), data.size() * sizeof(T)); }
	
	void use();
	
	inline GLIDX id() { return this->m_buffer_id; }
	
	const SILCON_GLBUFFER_TYPE buffer_type = BufferT;
	const SILCON_GLBUFFER_USAGE buffer_usage = UsageT;
	
private:
	
	GLIDX m_buffer_id = -1;
};

#endif /* defined(__MarXsCube__GLFoundation__) */
