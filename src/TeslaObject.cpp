//
//  TeslaObject.cpp
//  MarXsCube
//
//  Created by SeconDatke on 2/5/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "ATVBCube.h"
#include "GLShaderExt.h"
#include "GLFoundation.h"
#include "CubeTransform.h"
#include "Generic.h"
#include "TeslaBullet.h"
#include "TeslaObject.h"

#include "cubegl.h"
#include "BulletCollision/CollisionDispatch/btInternalEdgeUtility.h"

using namespace ATVBCube::Helper;

TeslaObject *TeslaObject::create() {
	// only used in Lua, so just pass the singleton
	return new TeslaObject(Generic::TeslaManger()); }

void TeslaObject::create_bullet() {
	transfer_bullet_shape(*this->m_bmesh, *this->m_bullet_model, this->m_tex_heightfield_s);

	this->m_bshape = new btBvhTriangleMeshShape(this->m_bmesh, true);
	this->m_bshape->setMargin(btScalar(0.1f));

	btTransform btrans;
	btrans.setIdentity();
	btrans.setOrigin(coord2bt(this->m_location));

	this->m_bmotion_state = new btDefaultMotionState(btrans);
	btRigidBody::btRigidBodyConstructionInfo grInfo(0., this->m_bmotion_state, this->m_bshape);
	this->m_bbody = new btRigidBody(grInfo);
	this->m_bbody->setRestitution(btScalar(0.5));
	this->m_bbody->setFriction(btScalar(0.8));
	this->m_bbody->setRollingFriction(1);
	this->m_bbody->setCollisionFlags(this->m_bbody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);

	this->m_bbody->setCollisionFlags(this->m_bbody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
	btGenerateInternalEdgeInfo(this->m_bshape, this->m_btri_map);

	this->m_bphyobj = new PhysicsObject(true);
	this->m_bphyobj->isCell = true;
	this->m_bbody->setUserPointer(this->m_bphyobj);
	Generic::PhysicsGeneral()->dynaWorld->addRigidBody(this->m_bbody);
}

void TeslaObject::load_shader() {
	this->m_shader_unique = new tesla_shader;
	this->m_shader_unique->load_file(SHADERTYPE::VERTEX, "shaders/tesla.vert");
	this->m_shader_unique->load_file(SHADERTYPE::FRAG, "shaders/tesla.frag");
	this->m_shader_unique->create();
	this->m_shader_unique->init_shader();

	this->m_shader = this->m_shader_unique;

	std::cout << m_shader->log(VERTEX);
	std::cout << m_shader->log(FRAG);
}

void TeslaObject::load_objfile(const std::string& path, const std::string& path_bullet) {
	this->m_basic_model->filepath = path;
	this->m_basic_model->parse();

	this->m_bullet_model->filepath = path_bullet;
	this->m_bullet_model->parse();

	transfer_verts_tesla(*this->m_mesh_data, *this->m_basic_model);
}

void TeslaObject::load_buffer() {
	this->m_mesh_data->vec_verts().reserve(this->m_mesh_data->vec_verts().size()*4.f);
	this->m_mesh_data->vec_indexes().reserve(this->m_mesh_data->vec_indexes().size()*4.f);

	this->m_buffer_vert->init_with(this->m_mesh_data->vec_verts(), 4.0f);
	this->m_buffer_idx->init_with(this->m_mesh_data->vec_indexes(), 4.0f);
}

void TeslaObject::Render() {
	this->m_buffer_vert->use();
	this->m_shader->use_n_load();

	SET_UNIFORMAT4P(this->m_shader_unique, model_view_and_projection, this->m_parent->m_mat_mvp);
	SET_UNIFORM3P(this->m_shader_unique, chunk_position, this->m_location);
	BIND_TEXTUREP(this->m_shader_unique, texture_main, this->m_tex_background, 0);
	glSamplerParameteri(0, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glSamplerParameteri(0, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	BIND_TEXTUREP(this->m_shader_unique, texture_heightfield, this->m_tex_heightfield, 2);
	BIND_TEXTUREP(this->m_shader_unique, texture_tileset, this->m_tex_tileset, 3);

	this->m_buffer_idx->use();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	SET_UNIFORMBP(this->m_shader_unique, is_wireframe, false);
	glDrawElements(GL_TRIANGLES, (int)this->m_mesh_data->count_idx(), GL_UNSIGNED_INT, 0);

	// wireframe rendering
	// WARNING: glPolygonMode is likely unsupported in OpenGL ES.
	if (ATVBCube::setting<S::TeslaGeneralSetting>().enable_wireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		SET_UNIFORMBP(this->m_shader_unique, is_wireframe, true);
		glDrawElements(GL_TRIANGLES, (int)this->m_mesh_data->count_idx(), GL_UNSIGNED_INT, 0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	GLFoundation::unbind(*this->m_buffer_idx);

	this->m_shader->disable_attributes();
	GLFoundation::unbind(*this->m_buffer_vert);
}
