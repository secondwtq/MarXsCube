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
		this->update();
	}
	
	void LineShape::setPoints(const Vector2f& pt1, const Vector2f& pt2) {
		this->setPosition(pt1);
		this->m_direction = pt2 - pt1;
		this->update();
	}
	
	void LineShape::setThickness(float thickness) {
		this->m_thickness = thickness;
		this->update();
	}
	
	float LineShape::getLength() const {
		return sqrt(m_direction.x*m_direction.x+m_direction.y*m_direction.y); }
	
	Vector2f LineShape::getPoint(unsigned int index) const {
		Vector2f u_dir = this->m_direction / this->getLength();
		Vector2f u_dirt(-u_dir.y, u_dir.x);
		Vector2f offset = this->m_thickness*u_dirt;
		
		switch (index) {
			default:
			case 0: return offset;
			case 1: return this->m_direction+offset;
			case 2: return this->m_direction-offset;
			case 3: return (-offset);
		}
	}
	
}