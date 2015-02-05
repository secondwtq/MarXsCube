//
//  GritFoundation.cpp
//  MarXsCube
//
//  Created by SeconDatke on 2/5/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "Common.h"
#include "GritFoundation.h"

#include <algorithm>
#include <vector>

#include <glm/glm.hpp>

#define GLVEC2(gpoint) (glm::vec2(((gpoint).x), ((gpoint).y)))
#define GVEC2(glpt) (GPointType((GUnitT)((glpt).x), (GUnitT)((glpt).y)));

bool Grit::pt_is_valid(const GPointType& pt) {
	
	return true;
}

bool Grit::pt_is_concave(const std::vector<GPointType>& pts, std::size_t pt) {
	GPointType cur = pts[pt], next = pts[(pt+1) % pts.size()],
				prev = pts[(!pt) ? pts.size()-1 : pt-1];
	GPointType left { cur.x - prev.x, cur.y - prev.y },
				right { next.x - cur.x, next.y - cur.y };
	
	return (left.x*right.y - left.y*right.x) > 0;
}

std::vector<GPointType> Grit::inflate_poly(const std::vector<GPointType>& pts, GUnitT dist) {
	std::vector<GPointType> ret;
	
	for (std::size_t i = 0; i < pts.size(); i++) {
		glm::vec2 ab = glm::normalize(GLVEC2(pts[(i+1) % pts.size()]) = GLVEC2(pts[i])),
			ac = glm::normalize(GLVEC2(pts[(!i) ? pts.size()-1 : i-1]) - GLVEC2(pts[i]));
		glm::vec2 mid = glm::normalize(ab + ac);
		
		mid *= (float)(pt_is_concave(pts, i) ? -dist : dist);
		GPointType offseted = pts[i];
		offseted += GVEC2(mid);
		ret.push_back(offseted);
	}
	
	return ret;
}

bool Grit::segments_cross(const GPointType& a, const GPointType& b, const GPointType& c, const GPointType &d) {
	GUnitT denominator = ((b.x - a.x) * (d.y - c.y)) - ((b.y - a.y) * (d.x - c.x));
	
	if (!denominator) return false;
	
	GUnitT numerator1 = ((a.y - c.y) * (d.x - c.x)) - ((a.x - c.x) * (d.y - c.y));
	GUnitT numerator2 = ((a.y - c.y) * (b.x - a.x)) - ((a.x - c.x) * (b.y - a.y));
	
	if ((!numerator1) || (!numerator2)) return false;
	
	float r = numerator1 / (float)denominator,
		s = numerator2 / (float)denominator;
	
	return (r > 0 && r < 1) && (s > 0 && s < 1);
}

bool Grit::pt_in_poly(const GPointType& pt, const std::vector<GPointType>& polypts) {
	GUnitT x_min = 0.f;
	for (auto p : polypts)
		x_min = std::min(x_min, p.x);
	
	GPointType origin { x_min - 1, pt.y };
	int intersections = 0;
	
	for (std::size_t i = 0; i < polypts.size(); i++) {
		GPointType pa = polypts[i];
		GPointType pb = polypts[(i+1) % polypts.size()];
		
		if (segments_cross(origin, pt, pa, pb))
			intersections++;
	}
	
	return (intersections & 1) == 1;
}