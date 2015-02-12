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
#include "Silcon.h"
#include <glm/glm.hpp>

class SilconSprite {
	
	friend void Silcon::SilconRenderingManger::render_dispatch();
	friend SilconSpriteVertex *SilconSpriteGeneral::vert_ref(SilconSprite *sprite, std::size_t offset);
	friend void Silcon::SilconRenderingManger::register_sprite(SilconSprite *sprite);
	
public:
	
	void update_size() {
		if (this->m_last_width != this->width || this->m_last_height != this->height) {
			float half_width = this->width/2.0f, half_height = this->height/2.0f;
			
			vert_ref(0)->position = vert_ref(5)->position = { -half_width, -half_height, 0 };
			vert_ref(1)->position = { half_width, -half_height, 0 };
			vert_ref(2)->position = vert_ref(3)->position = { half_width, half_height, 0 };
			vert_ref(4)->position = { -half_width, half_height, 0 };
			
			this->m_last_width = this->width, this->m_last_height = this->height;
		}
	}
	
	void set_texture_area(std::size_t left, std::size_t top, std::size_t width, std::size_t height) {
		this->width = width, this->height = height;
		this->update_size();
		
		float rwidth = width / this->m_tex_width, rheight = height / this->m_tex_height;
		float rtop = top / this->m_tex_height, rleft = left / this->m_tex_width;
		
		vert_ref(0)->texcoord = vert_ref(5)->texcoord = { rleft, rtop, 0 };
		vert_ref(1)->texcoord = { rleft+rwidth, rtop, 0 };
		vert_ref(2)->texcoord = vert_ref(3)->texcoord = { rleft+rwidth, rtop+rheight, 0 };
		vert_ref(4)->texcoord = { rleft, rtop+rheight, 0 };

	}
	
	void set_texture(const TextureAtlas& texture) {
		this->width = this->m_tex_width = texture.texture.getSize().x;
		this->height = this->m_tex_height = texture.texture.getSize().y;
		this->m_texture_id = texture.texture.m_texture;
		
		vert_ref(0)->texcoord = vert_ref(5)->texcoord = { 0, 0, 0 };
		vert_ref(1)->texcoord = { 1.f, 0, 0 };
		vert_ref(2)->texcoord = vert_ref(3)->texcoord = { 1.f, 1.f, 0 };
		vert_ref(4)->texcoord = { 0, 1.f, 0 };
		
		this->update_size();
	}
	
	void set_Zvalue(float offset) {
		this->m_zoffset = offset; }
	
	glm::vec2 position;
	glm::vec4 color_multiply { 1.0, 1.0, 1.0, 1.0 };
	float width, height;
	
private:
	
	float m_last_width, m_last_height;
	float m_tex_width, m_tex_height;
	
	std::size_t m_sprite_id = 0;
	float m_zoffset = 0;
	GLIDX m_texture_id = 0;
	
	inline SilconSpriteVertex *vert_ref(std::size_t offset) {
		return &SilconSpriteGeneral::m_global_verts[this->m_sprite_id*6+offset]; }
	
};

#endif /* defined(__MarXsCube__SilconSprite__) */
