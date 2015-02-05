//
//  TilerObject.cpp
//  MarXsCube
//
//  Created by SeconDatke on 2/5/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "GLShaderExt.h"
#include "GLFoundation.h"
#include "CubeTransform.h"
#include "Generic.h"
#include "TilerBullet.h"
#include "TilerObject.h"

#include <SFML/OpenGL.hpp>
#include "BulletCollision/CollisionDispatch/btInternalEdgeUtility.h"

void TilerObject::create_bullet() {
	transfer_bullet_shape(*this->m_bmesh, *this->m_basic_model, this->m_tex_heightfield_s);
	
	this->m_bshape = new btBvhTriangleMeshShape(this->m_bmesh, true);
	this->m_bshape->setMargin(btScalar(0.1f));
	
	this->m_bmotion_state = new btDefaultMotionState(this->m_btrans);
	btRigidBody::btRigidBodyConstructionInfo grInfo(0., this->m_bmotion_state, this->m_bshape);
	this->m_bbody = new btRigidBody(grInfo);
	this->m_bbody->setRestitution(btScalar(0.5));
	this->m_bbody->setFriction(btScalar(0.8));
	this->m_bbody->setCollisionFlags(this->m_bbody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	
	this->m_bbody->setCollisionFlags(this->m_bbody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
	btGenerateInternalEdgeInfo(this->m_bshape, this->m_btri_map);
	
	this->m_bphyobj = new PhysicsObject(true);
	this->m_bphyobj->isCell = true;
	this->m_bbody->setUserPointer(this->m_bphyobj);
	Generic::PhysicsGeneral()->dynaWorld->addRigidBody(this->m_bbody);
}

void TilerObject::load_shader() {
	this->m_shader_unique = new tiler_shader;
	this->m_shader_unique->load_file(SHADERTYPE::VERTEX, "terrain_tiler.vert");
	this->m_shader_unique->load_file(SHADERTYPE::FRAG, "terrain_tiler.frag");
	this->m_shader_unique->create();
	this->m_shader_unique->init_shader();
	
	this->m_shader = this->m_shader_unique;
}

void TilerObject::load_objfile(const std::string& path) {
	this->m_basic_model->filepath = path;
	this->m_basic_model->parse();
	
	transfer_verts_tiler(*this->m_mesh_data, *this->m_basic_model);
}

void TilerObject::load_buffer() {
	this->m_buffer_vert->init_with(this->m_mesh_data->vec_verts());
	this->m_buffer_idx->init_with(this->m_mesh_data->vec_indexes());
}

void TilerObject::Render() {
	glm::vec3 look_at_vec = CubeTransform::look_at_vector();
	GLFoundation::view(look_at_vec.x+this->m_location.x, look_at_vec.y+this->m_location.y);
	
	this->m_buffer_vert->use();
	
	this->m_shader->use_n_load();
	
	BIND_TEXTUREP(this->m_shader_unique, texture_heightfield, this->m_tex_heightfield, 2);
	BIND_TEXTUREP(this->m_shader_unique, texture_tileset, this->m_tex_tileset, 3);
	
	this->m_buffer_idx->use();
	glDrawElements(GL_TRIANGLES, (int)this->m_mesh_data->count_idx(), GL_UNSIGNED_INT, 0);
	GLFoundation::unbind(*this->m_buffer_idx);
	
	this->m_shader->disable_attributes();
	GLFoundation::unbind(*this->m_buffer_vert);
}