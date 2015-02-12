//
//  Silcon.cpp
//  MarXsCube
//
//  Created by SeconDatke on 2/12/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "Silcon.h"
#include "SilconShader.h"
#include <algorithm>

Silcon::SilconRenderingManger *Silcon::Manger = nullptr;

namespace Silcon {
	
	void SilconRenderingManger::render_dispatch() {
		
		// the Z offset is inversed for depth test
		//		so the operation is inversed too
		std::sort(m_sprites.begin(), m_sprites.end(),
			 [](const SilconSprite *a, const SilconSprite *b) -> bool {
				 return a->m_zoffset < b->m_zoffset; });
		
		SilconSpriteGeneral::pre_render();
		
		for (auto sprite : this->m_sprites) {
			glm::vec3 t { sprite->position.x, sprite->position.y, sprite->m_zoffset };
			SET_UNIFORM3(SilconSpriteGeneral::m_shader, sprite_position, t);
			SET_UNIFORM4(SilconSpriteGeneral::m_shader, color_multiply, sprite->color_multiply);
			BIND_TEXTURE(SilconSpriteGeneral::m_shader, texture_main, sprite->m_texture_id, 0);
			
			glDrawArrays(GL_TRIANGLES, static_cast<GLint>(6*sprite->m_sprite_id), 6);
		}
		
		SilconSpriteGeneral::post_render();
	}
	
	void SilconRenderingManger::register_sprite(SilconSprite *sprite) {
		this->m_sprites.push_back(sprite);
		sprite->m_sprite_id = SilconSpriteGeneral::get_a_sprite_id();
	}
	
}