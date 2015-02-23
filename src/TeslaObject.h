//
//  TeslaObject.h
//  MarXsCube
//
//  Created by SeconDatke on 2/5/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef __MarXsCube__TeslaObject__
#define __MarXsCube__TeslaObject__

#include "Common.h"
#include "SFML.h"
#include "PhysicsCommon.h"
#include "ModelLoader_obj.h"
#include "GLFoundation.h"
#include "GLShaderExt.h"
#include "TeslaRenderingBasic.h"

#include <string>

class PhysicsObject;
class TeslaRenderingManger;

class TeslaObject {
public:
	
	TeslaObject (TeslaRenderingManger *parent) : m_parent(parent) {
		this->m_buffer_vert = new gl_buffer<VBO, DYNAMIC>;
		this->m_buffer_idx = new gl_buffer<IBO, DYNAMIC>;
		this->m_basic_model = new objfile;
		this->m_bullet_model = new objfile;
		this->m_mesh_data = new tesla_dataarray;
		this->m_btri_map = new btTriangleInfoMap();
		this->m_bmesh = new btTriangleMesh();
		
		this->m_buf_bgpaint = new gl_buffer<VBO, DYNAMIC>;
	}
	
	void location(const CoordStruct& src) {
		this->m_location = src; }
	
	void Render();
	
	void set_tileset(const TextureAtlas *texture) {
		this->m_tex_tileset = texture->texture.m_texture; }
	
	void set_heightfield(const TextureAtlas *texture) {
		this->m_tex_heightfield_s = &(texture->texture);
		this->m_tex_heightfield = texture->texture.m_texture; }
	
	void set_background(const TextureAtlas *texture) {
		this->m_tex_background = texture->texture.m_texture; }
	
	void load_objfile(const std::string& path, const std::string& path_bullet);
	
	void load_shader();
	
	void create_bullet();
	
	void load_buffer();
	
	tesla_dataarray *get_data() { return this->m_mesh_data; }
	gl_buffer<VBO, DYNAMIC> *buffer_vert() { return this->m_buffer_vert; }
	gl_buffer<IBO, DYNAMIC> *buffer_idx() { return this->m_buffer_idx; }
	
	static TeslaObject *create();
	
	GLIDX m_tex_background = -1;
	GLIDX m_fb_bgpaint = -1;
	gl_buffer<VBO, DYNAMIC> *m_buf_bgpaint = nullptr;
	
private:
	
	CoordStruct m_location { 0, 0, 0 };
	
	GLIDX m_tex_heightfield = -1;
	GLIDX m_tex_tileset = -1;
	const sf::Texture *m_tex_heightfield_s = nullptr;
	
	gl_buffer<VBO, DYNAMIC> *m_buffer_vert = nullptr;
	gl_buffer<IBO, DYNAMIC> *m_buffer_idx = nullptr;
	
	gl_shader_ext *m_shader = nullptr;
	tesla_shader *m_shader_unique = nullptr;
	
	objfile *m_basic_model = nullptr;
	objfile *m_bullet_model = nullptr;
	tesla_dataarray *m_mesh_data = nullptr;
	
	btTriangleMesh *m_bmesh = nullptr;
	btBvhTriangleMeshShape *m_bshape = nullptr;
	btTriangleInfoMap *m_btri_map = nullptr;
	btRigidBody *m_bbody = nullptr;
	btDefaultMotionState *m_bmotion_state = nullptr;
	PhysicsObject *m_bphyobj = nullptr;
	
	TeslaRenderingManger *m_parent = nullptr; // not to be freed
};

#include "TeslaRendering.h"

#include "Physics.h"

#endif /* defined(__MarXsCube__TeslaObject__) */
