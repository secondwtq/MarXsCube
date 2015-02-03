//
//  BulletDebugDraw.cpp
//  MarXsCube
//
//  Created by SeconDatke on 2/3/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "Common.h"

#include "GLFoundation.h"
#include "Generic.h"
#include "BulletDebugDraw.h"

#include <SFML/OpenGL.hpp>

// reference: from Bullet official demos
// TODO: scaling and coord unfinished

#define DEBUG_DRAW_SCALE 3

void BulletDebugDrawer::render() {
	GLFoundation::unbind_shader();
	glDisable(GL_DEPTH_TEST);
	Generic::PhysicsGeneral()->dynaWorld->debugDrawWorld();
	glEnable(GL_DEPTH_TEST);
}

void BulletDebugDrawer::drawLine(const btVector3& from, const btVector3& to,
								 const btVector3& fromColor, const btVector3& toColor) {
	glBegin(GL_LINES);
	{
		btVector3 sfrom = from * PHY_SCALE_INV * DEBUG_DRAW_SCALE,
					sto = to * PHY_SCALE_INV * DEBUG_DRAW_SCALE;
		glColor3f(fromColor.getX(), fromColor.getY(), fromColor.getZ());
		glVertex3d(sfrom.getY(), sfrom.getX(), sfrom.getZ());
		glColor3f(toColor.getX(), toColor.getY(), toColor.getZ());
		glVertex3d(sto.getY(), sto.getX(), sto.getZ());
	}
	glEnd();
}

void BulletDebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color) {
	drawLine(from,to,color,color); }

void BulletDebugDrawer::drawSphere (const btVector3& p, btScalar radius, const btVector3& color) {
	glColor4f(color.getX(), color.getY(), color.getZ(), btScalar(1.0f));
	glPushMatrix();
	glTranslatef(p.getX(), p.getY(), p.getZ());
	
	int lats = 5;
	int longs = 5;
	
	int i, j;
	for(i = 0; i <= lats; i++) {
		btScalar lat0 = SIMD_PI * (-btScalar(0.5) + (btScalar) (i - 1) / lats);
		btScalar z0  = radius*sin(lat0);
		btScalar zr0 =  radius*cos(lat0);
		
		btScalar lat1 = SIMD_PI * (-btScalar(0.5) + (btScalar) i / lats);
		btScalar z1 = radius*sin(lat1);
		btScalar zr1 = radius*cos(lat1);
		
		glBegin(GL_QUAD_STRIP);
		for(j = 0; j <= longs; j++) {
			btScalar lng = 2 * SIMD_PI * (btScalar) (j - 1) / longs;
			btScalar x = cos(lng);
			btScalar y = sin(lng);
			
			glNormal3f(x * zr0, y * zr0, z0);
			glVertex3f(x * zr0, y * zr0, z0);
			glNormal3f(x * zr1, y * zr1, z1);
			glVertex3f(x * zr1, y * zr1, z1);
		}
		glEnd();
	}
	
	glPopMatrix();
}



void BulletDebugDrawer::drawTriangle(const btVector3& a, const btVector3& b,
										const btVector3& c, const btVector3& color, btScalar alpha) {
	const btVector3	n=btCross(b-a,c-a).normalized();
	btVector3 sa = a * PHY_SCALE_INV,
			sb = b * PHY_SCALE_INV,
			sc = c * PHY_SCALE_INV;
	
	
	glBegin(GL_TRIANGLES);
	glColor4f(color.getX(), color.getY(), color.getZ(),alpha);
	glNormal3d(n.getX(),n.getY(),n.getZ());
	glVertex3d(sa.getX(),sa.getY(),sa.getZ());
	glVertex3d(sb.getX(),sb.getY(),sb.getZ());
	glVertex3d(sc.getX(),sc.getY(),sc.getZ());
	glEnd();
}

void BulletDebugDrawer::draw3dText(const btVector3& location,const char* textString)
{
	glRasterPos3f(location.x(),  location.y(),  location.z());
}

void BulletDebugDrawer::reportErrorWarning(const char* warningString) {
	printf("%s\n",warningString);
}

void BulletDebugDrawer::drawContactPoint(const btVector3& pointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) {
		btVector3 to=pointOnB+normalOnB*1;//distance;
		const btVector3&from = pointOnB;
		glColor4f(color.getX(), color.getY(), color.getZ(),1.f);
		//glColor4f(0,0,0,1.f);
		glBegin(GL_LINES);
		glVertex3d(from.getX(), from.getY(), from.getZ());
		glVertex3d(to.getX(), to.getY(), to.getZ());
		glEnd();
}