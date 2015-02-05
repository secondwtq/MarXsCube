//
//  TilerObject.h
//  MarXsCube
//
//  Created by SeconDatke on 2/5/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef __MarXsCube__TilerObject__
#define __MarXsCube__TilerObject__

#include "Common.h"
#include "SFML.h"
#include "PhysicsCommon.h"
#include "ModelLoader_obj.h"
#include "GLFoundation.h"
#include "GLShaderExt.h"
#include "TilerRenderingBasic.h"

#include <string>

class PhysicsObject;

class TilerObject {
public:
	
	TilerObject () {
		this->m_buffer_vert = new gl_buffer<VBO, DYNAMIC>;
		this->m_buffer_idx = new gl_buffer<IBO, DYNAMIC>;
		this->m_basic_model = new objfile;
		this->m_mesh_data = new tiler_dataarray;
		this->m_btri_map = new btTriangleInfoMap();
		this->m_bmesh = new btTriangleMesh();
		this->m_btrans.setIdentity();
	}
	
	void location(const CoordStruct& src) {
		this->m_location = src;
		this->m_btrans.setIdentity();
		this->m_btrans.setOrigin(coord2bt(this->m_location));
	}
	
	void Render();
	
	void set_tileset(const TextureAtlas *texture) {
		this->m_tex_tileset = texture->texture.m_texture; }
	
	void set_heightfield(const TextureAtlas *texture) {
		this->m_tex_heightfield_s = &(texture->texture);
		this->m_tex_heightfield = texture->texture.m_texture; }
	
	void load_objfile(const std::string& path);
	
	void load_shader();
	
	void create_bullet();
	
	void load_buffer();
	
	static TilerObject *create() { return new TilerObject(); }
	
private:
	
	CoordStruct m_location { 0, 0, 0 };
	btTransform m_btrans;
	
	GLIDX m_tex_heightfield = -1;
	GLIDX m_tex_tileset = -1;
	const sf::Texture *m_tex_heightfield_s = nullptr;
	
	gl_buffer<VBO, DYNAMIC> *m_buffer_vert = nullptr;
	gl_buffer<IBO, DYNAMIC> *m_buffer_idx = nullptr;
	
	gl_shader_ext *m_shader = nullptr;
	tiler_shader *m_shader_unique = nullptr;
	
	objfile *m_basic_model = nullptr;
	tiler_dataarray *m_mesh_data = nullptr;
	
	btTriangleMesh *m_bmesh = nullptr;
	btBvhTriangleMeshShape *m_bshape = nullptr;
	btTriangleInfoMap *m_btri_map = nullptr;
	btRigidBody *m_bbody = nullptr;
	btDefaultMotionState *m_bmotion_state = nullptr;
	PhysicsObject *m_bphyobj = nullptr;
};

#include "Physics.h"

#endif /* defined(__MarXsCube__TilerObject__) */
