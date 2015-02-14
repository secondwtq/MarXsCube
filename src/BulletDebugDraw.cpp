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

std::vector<BulletDebugVertex> BulletDebugDrawer::m_buffer_vec;
gl_buffer<VBO, DYNAMIC> BulletDebugDrawer::m_buffer;
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
	
	m_buffer.init_with(m_buffer_vec.data(), 30000*sizeof(BulletDebugVertex));
}

void BulletDebugDrawer::render() {
	GLenum t = glGetError();
	glDisable(GL_BLEND);
	GLFoundation::unbind_shader();
	
	m_buffer_vec.clear();
	
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearDepth(1.0f);
	Generic::PhysicsGeneral()->dynaWorld->debugDrawWorld();
	m_buffer.use();
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_buffer_vec.size() * sizeof(BulletDebugVertex), m_buffer_vec.data());
	t = glGetError();
	GLFoundation::unbind(m_buffer);
	
	m_buffer.use();
	m_shader.use_n_load();
	SET_UNIFORMAT4(m_shader, model_view_and_projection, Generic::TeslaManger()->m_mat_mvp);
	glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(m_buffer_vec.size()));
	GLFoundation::unbind_shader();
	GLFoundation::unbind(m_buffer);
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
	drawLine(from,to,color,color); }

void BulletDebugDrawer::drawSphere (const btVector3& p, btScalar radius, const btVector3& color) {
//	glColor4f(color.getX(), color.getY(), color.getZ(), btScalar(1.0f));
//	glPushMatrix();
//	glTranslatef(p.getX(), p.getY(), p.getZ());
//	
//	int lats = 5;
//	int longs = 5;
//	
//	int i, j;
//	for(i = 0; i <= lats; i++) {
//		btScalar lat0 = SIMD_PI * (-btScalar(0.5) + (btScalar) (i - 1) / lats);
//		btScalar z0  = radius*sin(lat0);
//		btScalar zr0 =  radius*cos(lat0);
//		
//		btScalar lat1 = SIMD_PI * (-btScalar(0.5) + (btScalar) i / lats);
//		btScalar z1 = radius*sin(lat1);
//		btScalar zr1 = radius*cos(lat1);
//		
//		glBegin(GL_QUAD_STRIP);
//		for(j = 0; j <= longs; j++) {
//			btScalar lng = 2 * SIMD_PI * (btScalar) (j - 1) / longs;
//			btScalar x = cos(lng);
//			btScalar y = sin(lng);
//			
//			glNormal3f(x * zr0, y * zr0, z0);
//			glVertex3f(x * zr0, y * zr0, z0);
//			glNormal3f(x * zr1, y * zr1, z1);
//			glVertex3f(x * zr1, y * zr1, z1);
//		}
//		glEnd();
//	}
//	
//	glPopMatrix();
}



void BulletDebugDrawer::drawTriangle(const btVector3& a, const btVector3& b,
										const btVector3& c, const btVector3& color, btScalar alpha) {
//	const btVector3	n=btCross(b-a,c-a).normalized();
//	btVector3 sa = a * PHY_SCALE_INV,
//			sb = b * PHY_SCALE_INV,
//			sc = c * PHY_SCALE_INV;
//	
//	
//	glBegin(GL_TRIANGLES);
//	glColor4f(color.getX(), color.getY(), color.getZ(),alpha);
//	glNormal3d(n.getX(),n.getY(),n.getZ());
//	glVertex3d(sa.getX(),sa.getY(),sa.getZ());
//	glVertex3d(sb.getX(),sb.getY(),sb.getZ());
//	glVertex3d(sc.getX(),sc.getY(),sc.getZ());
//	glEnd();
}

void BulletDebugDrawer::draw3dText(const btVector3& location,const char* textString)
{
//	glRasterPos3f(location.x(),  location.y(),  location.z());
}

void BulletDebugDrawer::reportErrorWarning(const char* warningString) {
	printf("%s\n",warningString);
}

void BulletDebugDrawer::drawContactPoint(const btVector3& pointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) {
//		btVector3 to=pointOnB+normalOnB*1;//distance;
//		const btVector3&from = pointOnB;
//		glColor4f(color.getX(), color.getY(), color.getZ(),1.f);
//		//glColor4f(0,0,0,1.f);
//		glBegin(GL_LINES);
//		glVertex3d(from.getX(), from.getY(), from.getZ());
//		glVertex3d(to.getX(), to.getY(), to.getZ());
//		glEnd();
}

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