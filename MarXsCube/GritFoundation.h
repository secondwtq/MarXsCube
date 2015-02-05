//
//  GritFoundation.h
//  MarXsCube
//
//  Created by SeconDatke on 2/5/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef __MarXsCube__GritFoundation__
#define __MarXsCube__GritFoundation__

#include "Common.h"

#include <vector>

typedef CubePoint GPointType;
typedef int GUnitT;

class Grit {
	static bool pt_is_valid(const GPointType& pt);
	static std::vector<GPointType> inflate_poly(const std::vector<GPointType>& pts, GUnitT dist);
	static bool segments_cross(const GPointType& a, const GPointType& b, const GPointType& c, const GPointType &d);
	static bool pt_is_concave(const std::vector<GPointType>& pts, std::size_t pt);
	static bool pt_in_poly(const GPointType& pt, const std::vector<GPointType>& polypts);
};

class GritPoly {
public:
	std::vector<CubePoint> pts;
	
	GritPoly(const std::vector<CubePoint>& points) :
		pts(points) { }
};

class GritPolyMap {
public:
	std::vector<GritPoly *> master_polys;
	std::vector<GritPoly *> obs_polys;
	
	std::vector<GritPoly *> all_polys() {
		std::vector<GritPoly *> ret;
		
		return ret;
	}
	
	GritPolyMap(const std::vector<GritPoly *>& masterp, const std::vector<GritPoly *>& obsp) :
		master_polys(masterp), obs_polys(obsp) { }
};

class GritNode {
public:
	
	CubePoint pos;
	std::vector<int> links;
	float cost = 1.f;
	float cost_est = 0.f;
	GritNode *parent = nullptr;
	
	GritNode(const CubePoint& location) :
		pos(location) { }
	
	void compare() { }
	void clone() { }
};

#endif /* defined(__MarXsCube__GritFoundation__) */
