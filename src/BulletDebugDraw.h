//
//  BulletDebugDraw.h
//  MarXsCube
//
//  Created by SeconDatke on 2/3/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef __MarXsCube__BulletDebugDraw__
#define __MarXsCube__BulletDebugDraw__

#include "LinearMath/btIDebugDraw.h"
#include "GLFoundation.h"
#include "BulletDebugShader.h"

#include <vector>
#include <functional>

// reference: from Bullet official demos
class BulletDebugDrawer : public btIDebugDraw {

public:
	
	BulletDebugDrawer();
	virtual ~BulletDebugDrawer() { }
	
	virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor);
	
	virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
	
	virtual void drawSphere (const btVector3& p, btScalar radius, const btVector3& color) { }
	
	virtual void drawTriangle(const btVector3& a, const btVector3& b, const btVector3& c,const btVector3& color, btScalar alpha) { }
	
	virtual void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB,btScalar distance, int lifeTime, const btVector3& color) { }
	
	virtual void reportErrorWarning(const char* warningString);
	
	virtual void draw3dText(const btVector3& location, const char* textString) { }
	
	virtual void setDebugMode(int debugMode) { this->m_debugMode = debugMode; }
	
	virtual int getDebugMode() const { return this->m_debugMode; }
	
	void set_draw_scale(float scale) { this->m_draw_scale = scale; }
	
	void push_task(std::function<void (BulletDebugDrawer *)> foo) {
		this->m_task_queue.push_back(foo); }
	
	static void render();
	
	private:

		int m_debugMode;
		float m_draw_scale = 3.0f;
	
		std::vector<BulletDebugVertex> m_buffer_vec;
		gl_buffer<VBO, DYNAMIC> m_buffer;
	
		std::vector<std::function<void (BulletDebugDrawer *)> > m_task_queue;
	
		static BulletDebugShader m_shader;
	
};

#endif /* defined(__MarXsCube__BulletDebugDraw__) */
