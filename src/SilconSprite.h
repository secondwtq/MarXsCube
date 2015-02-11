//
//  SilconSprite.h
//  MarXsCube
//
//  Created by SeconDatke on 2/11/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef __MarXsCube__SilconSprite__
#define __MarXsCube__SilconSprite__

#include "SFML.h"
#include "GLFoundation.h"
#include "SilconCommon.h"
#include "SilconShader.h"
#include <glm/glm.hpp>

class SilconSprite;

namespace SilconSpriteGeneral {

	void init();
	
	extern std::vector<SilconSpriteVertex> m_global_verts;
	extern gl_buffer<VBO, DYNAMIC> m_global_buffer;
	extern SilconShader m_shader;
	
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
		return &m_global_verts[id*6+offset];
	}
	
	void pre_render();
	void post_render();
	
	SilconSpriteVertex *vert_ref(SilconSprite *sprite, std::size_t offset);
	
};

class SilconSprite {
	
	friend SilconSpriteVertex *SilconSpriteGeneral::vert_ref(SilconSprite *sprite, std::size_t offset);
	
public:
	
	void reg_sprite() { this->m_sprite_id = SilconSpriteGeneral::get_a_sprite_id(); }
	void set_texture(const TextureAtlas& texture) {
		this->width = texture.texture.getSize().x;
		this->height = texture.texture.getSize().y;
		this->m_texture_id = texture.texture.m_texture;
		
		using namespace SilconSpriteGeneral;
		
		float half_width = this->width/2.0f, half_height = this->height/2.0f;
		
		vert_ref(0)->position = vert_ref(5)->position = { -half_width, -half_height, 0 };
		vert_ref(0)->texcoord = vert_ref(5)->texcoord = { 0, 0, 0 };
		
		vert_ref(1)->position = { half_width, -half_height, 0 };
		vert_ref(1)->texcoord = { 1.f, 0, 0 };
		
		vert_ref(2)->position = vert_ref(3)->position = { half_width, half_height, 0 };
		vert_ref(2)->texcoord = vert_ref(3)->texcoord = { 1.f, 1.f, 0 };
		
		vert_ref(4)->position = { -half_width, half_height, 0 };
		vert_ref(4)->texcoord = { 0, 1.f, 0 };
		
	}
	
	void render();
	
	glm::vec2 position;
	glm::vec4 color_multiply { 1.0, 1.0, 1.0, 1.0 };
	
private:
	
	std::size_t width, height;
	std::size_t m_sprite_id = 0;
	GLIDX m_texture_id = 0;
	
	inline SilconSpriteVertex *vert_ref(std::size_t offset) {
		return &SilconSpriteGeneral::m_global_verts[this->m_sprite_id*6+offset]; }
	
};

#endif /* defined(__MarXsCube__SilconSprite__) */
