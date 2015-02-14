//
//  BulletDebugDraw.cpp
//  MarXsCube
//
//  Created by SeconDatke on 2/3/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "Common.h"
#include "FSM.h"

#include "GLFoundation.h"
#include "Generic.h"
#include "BulletDebugShader.h"
#include "BulletDebugDraw.h"
#include "GLShaderExtDef.h"

#include "cubegl.h"

using namespace FSMHelper;

BulletDebugShader BulletDebugDrawer::m_shader;

// reference: from Bullet official demos
// TODO: scaling and coord unfinished

BulletDebugDrawer::BulletDebugDrawer() {
	m_shader.load_file(SHADERTYPE::VERTEX, "shaders/bdebug.vert");
	m_shader.load_file(SHADERTYPE::FRAG, "shaders/bdebug.frag");
	m_shader.create();
	m_shader.init_shader();
	
	Generic::corelog() << "Bullet Debug Draw Shader Error Log: " << rn;
	std::cout << m_shader.log(VERTEX);
	std::cout << m_shader.log(FRAG);
	
	m_buffer.init_with(m_buffer_vec.data(), 40000*sizeof(BulletDebugVertex));
}

void BulletDebugDrawer::render() {
	BulletDebugDrawer *drawer = reinterpret_cast<BulletDebugDrawer *>(Generic::PhysicsGeneral()->dynaWorld->getDebugDrawer());
	
	glDisable(GL_BLEND);
	
	drawer->m_buffer_vec.clear();
	
	GLFoundation::clear_depth();
	Generic::PhysicsGeneral()->dynaWorld->debugDrawWorld();
	drawer->m_buffer.use();
	glBufferSubData(GL_ARRAY_BUFFER, 0, drawer->m_buffer_vec.size() * sizeof(BulletDebugVertex), drawer->m_buffer_vec.data());
	GLFoundation::unbind(drawer->m_buffer);
	
	GLFoundation::unbind_shader();
	drawer->m_buffer.use();
	m_shader.use_n_load();
	SET_UNIFORMAT4(m_shader, model_view_and_projection, Generic::TeslaManger()->m_mat_mvp);
	glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(drawer->m_buffer_vec.size()));
	GLFoundation::unbind_shader();
	GLFoundation::unbind(drawer->m_buffer);
	m_shader.disable_attributes();
}

void BulletDebugDrawer::drawLine(const btVector3& from, const btVector3& to,
								 const btVector3& fromColor, const btVector3& toColor) {
	btVector3 sfrom = from * PHY_SCALE_INV * m_draw_scale,
				sto = to * PHY_SCALE_INV * m_draw_scale;
	
	m_buffer_vec.push_back({ { sfrom.getY(), sfrom.getX(), sfrom.getZ() }, { fromColor.getX(), fromColor.getY(), fromColor.getZ()} });
	m_buffer_vec.push_back({ { sto.getY(), sto.getX(), sto.getZ() }, { toColor.getX(), toColor.getY(), toColor.getZ() } });
}

void BulletDebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color) {
	drawLine(from, to, color, color); }

void BulletDebugDrawer::reportErrorWarning(const char* warningString) {
	printf("%s\n", warningString); }

void BulletDebugShader::init_shader() {
	DEF_ATTRIBUTE_NT(position);
	DEF_ATTRIBUTE_NT(color);
	
	DEF_UNIFORM(model_view_and_projection);
}

void BulletDebugShader::attribute_attr() {
	
	SET_ATTRIBUTE3_NT(position, 0, 0);
	SET_ATTRIBUTE3_NT(color, 1, 3);
	
}

void BulletDebugShader::disable_attributes() {
	
	DISABLE_ATTRIBUTE_NT(position, 0);
	DISABLE_ATTRIBUTE_NT(color, 1);
	
}