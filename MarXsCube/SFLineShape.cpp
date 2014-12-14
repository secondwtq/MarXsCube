//
//  SFLineShape.cpp
//  MarXsCube
//
//  Created by SeconDatke on 12/15/14.
//  Copyright (c) 2014 MarXsCube Staff. All rights reserved.
//

#include "SFML.h"
#include <cmath>
#include "SFLineShape.h"

namespace sf {

	LineShape::LineShape(const Vector2f &pt1, const Vector2f& pt2) {
		this->setPoints(pt1, pt2);
	}
	
	void LineShape::setPoints(const Vector2f& pt1, const Vector2f& pt2) {
		this->setPosition(pt1);
		this->m_direction = pt2 - pt1;
		this->regen_pts();
	}
	
	void LineShape::setThickness(float thickness) {
		if (thickness != this->m_thickness) {
			this->m_thickness = thickness;
			this->regen_pts();
		}
	}
	
	float LineShape::len() const {
		return sqrt(m_direction.x*m_direction.x+m_direction.y*m_direction.y); }
	
	void LineShape::regen_pts() {
		Vector2f u_dir = this->m_direction / this->len();
		Vector2f u_dirt(-u_dir.y, u_dir.x);
		Vector2f offset = this->m_thickness*u_dirt;
		
		this->b_pts[0] = offset;
		this->b_pts[1] = this->m_direction+offset;
		this->b_pts[2] = this->m_direction-offset;
		this->b_pts[3] = -offset;
	}
	
	Vector2f LineShape::getPoint(unsigned int index) const {
		return this->b_pts[index];
//		Vector2f u_dir = this->m_direction / this->len();
//		Vector2f u_dirt(-u_dir.y, u_dir.x);
//		Vector2f offset = this->thickness*u_dirt;
//		
//		switch (index) {
//			default:
//			case 0: return offset;
//			case 1: return this->m_direction+offset;
//			case 2: return this->m_direction-offset;
//			case 3: return -offset;
//		}
	}
	
}