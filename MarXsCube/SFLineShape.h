//
//  SFLineShape.h
//  MarXsCube
//
//  Created by SeconDatke on 12/15/14.
//  Copyright (c) 2014 MarXsCube Staff. All rights reserved.
//

#ifndef __MarXsCube__SFLineShape__
#define __MarXsCube__SFLineShape__

#include "SFML.h"

// source from
//	https://github.com/LaurentGomila/SFML/wiki/Source:-Line-segment-with-thickness
//
namespace sf {
	class LineShape: public Shape {
	public:
		explicit LineShape(const Vector2f& pt1, const Vector2f& pt2);

		void setPoints(const Vector2f& pt1, const Vector2f& pt2);

		float getLength() const;

		void setThickness(float thickness);
		float getThickness() const { return this->m_thickness; };

		virtual unsigned int getPointCount() const { return 4; };
		virtual Vector2f getPoint(unsigned int index) const;
		float m_thickness = 10;

	private:
		Vector2f m_direction;
	};
}

#endif /* defined(__MarXsCube__SFLineShape__) */
