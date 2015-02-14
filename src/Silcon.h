//
//  Silcon.h
//  MarXsCube
//
//  Created by SeconDatke on 2/12/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef __MarXsCube__Silcon__
#define __MarXsCube__Silcon__

#include "SilconCommon.h"
#include <vector>

class SilconSprite;

namespace Silcon {
	
	class SilconRenderingManger;
	
	extern SilconRenderingManger *Manger;
	
	class SilconRenderingManger {
	public:
		
		void render_dispatch();
		
		void register_sprite(SilconSprite *sprite);
		
	private:
		std::vector<SilconSprite *> m_sprites;
	};
	
}

#include "GLFoundation.h"
#include "SilconShader.h"
#include <glm/glm.hpp>

namespace SilconSpriteGeneral {
	
	void init();
	
	extern std::vector<SilconSpriteVertex> m_global_verts;
	extern gl_buffer<VBO, DYNAMIC> m_global_buffer;
	extern SilconShader m_shader;
	extern glm::mat4 m_mat_mvp;
	
	inline void push_empty_sprite() {
		for (std::size_t i = 0; i < 6; i++) {
			m_global_verts.push_back({ });
		}
	}
	
	inline std::size_t get_a_sprite_id() {
		std::size_t ret = m_global_verts.size() / 6;
		push_empty_sprite();
		return ret;
	}
	
	inline SilconSpriteVertex *vert_ref(std::size_t id, std::size_t offset) {
		return &m_global_verts[id*6+offset]; }
	
	void pre_render();
	void post_render();
	
	SilconSpriteVertex *vert_ref(SilconSprite *sprite, std::size_t offset);
	
};

#include "SilconSprite.h"

#endif /* defined(__MarXsCube__Silcon__) */
