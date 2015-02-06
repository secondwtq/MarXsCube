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
#include "GritMath.h"

#include <algorithm>
#include <vector>

typedef CubePoint GPointType;
typedef int GUnitT;

class GritPoly;
class GritNode;
class GritPolyMap;

class GritObstacle {
public:
	std::vector<GPointType> pts;
};

class Grit {
public:
	bool pt_is_valid(const GPointType& pt);
	static std::vector<GPointType> inflate_poly(const std::vector<GPointType>& pts, GUnitT dist);
	static bool segments_cross(const GPointType& a, const GPointType& b, const GPointType& c, const GPointType &d);
	static bool pt_is_concave(const std::vector<GPointType>& pts, std::size_t pt);
	static bool pt_in_poly(const GPointType& pt, const std::vector<GPointType>& polypts);
	bool check_los(const GPointType& pa, const GPointType& pb);
	void link_node(GritNode& node, std::vector<GritNode *>& to_nodes);
	
	void generate_map();
	
	void create_polymap();
	void create_nodes();
	void link_nodes(const std::vector<GritNode *>& node_list);
	
	void late_update();
	
	void add_obs(GritObstacle *obs) {
		if (std::find(this->m_obses.begin(), this->m_obses.end(), obs) == this->m_obses.end()) {
			this->m_obses.push_back(obs);
			this->m_flag_generate = true;
		}
	}
	
	void remove_obs(GritObstacle *obs) {
		this->m_flag_generate = true;
	}
	
	std::vector<GritNode *> m_nodes;
	std::vector<GritObstacle *> m_obses;
	GritPolyMap *m_map;
	bool m_flag_generate;
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
	std::vector<std::size_t> links;
	float cost = 1.f;
	float cost_est = 0.f;
	GritNode *parent = nullptr;
	
	GritNode(const CubePoint& location) :
		pos(location) { }
	
	bool operator==(const GritNode& other) {
		return this->cost_est == other.cost_est; }
	
	void compare() { }
	void clone() { }
};

#endif /* defined(__MarXsCube__GritFoundation__) */
