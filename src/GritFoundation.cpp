//
//  GritFoundation.cpp
//  MarXsCube
//
//  Created by SeconDatke on 2/5/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "Common.h"
#include "Platform.h"
#include "FSM.h"
#include "GritFoundation.h"
#include "GritAStar.h"
#include "Generic.h"

#include <algorithm>
#include <vector>

#include <glm/glm.hpp>

using namespace FSMHelper;

#define GLVEC2(gpoint) (glm::vec2(((gpoint).x), ((gpoint).y)))
#define GVEC2(glpt) (GPointType((GUnitT)((glpt).x), (GUnitT)((glpt).y)));

std::vector<GPointType> Grit::find_path(const GPointType& start, const GPointType& end) {
	std::vector<GPointType> ret;
	
//	Generic::corelog() << "find_path: Checking los " << start.x << " " << start.y << " " << end.x << " " << end.y << rn;
	if (check_los(start, end)) {
		ret.push_back(end);
		ret.push_back(start);
		printf("Find path: returning (direct) ... \n");
		return ret;
	}
//	Generic::corelog() << "find_path: los failed, needs more work..." << rn;
	
	std::vector<GritNode *> tmp_nodes;
	for (auto node : this->m_nodes)
		tmp_nodes.push_back(node->clone());
	
//	GPointType start_org = start;
	if (!pt_is_valid(start)) {
		printf("find_path: point is not valid!");
	}
	
	GritNode *node_start = new GritNode(start),
			*node_end = new GritNode(end);
	
	tmp_nodes.push_back(node_start);
	tmp_nodes.push_back(node_end);
	
	link_node(*node_start, tmp_nodes);
	link_node(*node_end, tmp_nodes);
	
//	Generic::corelog() << "find_path: finding path... totally " << tmp_nodes.size() << " nodes" << rn;
	GritAStar::path(node_start, node_end, &tmp_nodes, &ret);
//	Generic::corelog() << "find_path: finished, " << ret.size() << " nodes" << rn;
	return ret;
}

void Grit::late_update() {
	if (this->m_flag_generate) {
		this->m_flag_generate = false;
		this->generate_map();
	}
}

void Grit::generate_map() {
	Generic::corelog() << "regererating map" << rn;
	this->create_polymap();
	this->create_nodes();
	
	Generic::corelog() << "linking nodes" << rn;
	this->link_nodes(this->m_nodes);
	
	Generic::corelog() << "poly counts: " <<  this->m_map->all_polys().size() << rn;
}

void Grit::create_polymap() {
	
	std::vector<GritPoly *> obs_polys;
	
	for (auto obs : this->m_obses) {
		// magic
		std::vector<GPointType> pts_inflated = inflate_poly(obs->pts, 16);
		obs_polys.push_back(new GritPoly(pts_inflated));
	}
	
	std::vector<GritPoly *> master_polys;
	// WIP.
	
	// creation
	this->m_map = new GritPolyMap(master_polys, obs_polys);
}

// finished. ( all_poly issue
void Grit::create_nodes() {
	Generic::corelog() << "regenerating nodes" << rn;
	this->m_nodes.clear();
	
	const std::vector<GritPoly *> vect = this->m_map->all_polys();
	for (std::size_t i = 0; i < vect.size(); i++) {
		// magic?
		Generic::corelog() << "handling polygon" << rn;
		std::vector<GPointType> pts_inflated = this->inflate_poly(vect[i]->pts, 8);
		for (std::size_t i = 0; i < pts_inflated.size(); i++) {
			Generic::corelog() << "handling point" << rn;
			if (pt_is_concave(pts_inflated, i)) {
				Generic::corelog() << "skipping because of concave point" << rn;
				continue;
			}
			if (!pt_is_valid(pts_inflated[i])) {
				Generic::corelog() << "skipping because of invalid point" << rn;
				continue;
			}
			
			// creation
			Generic::corelog() << "adding point" << rn;
			this->m_nodes.push_back(new GritNode(pts_inflated[i]));
		}
	}
}

// finished.
void Grit::link_nodes(const std::vector<GritNode *>& node_list) {
	for (std::size_t i = 0; i < node_list.size(); i++) {
		node_list[i]->links.clear();
		
		for (std::size_t j = 0; j < node_list.size(); j++) {
			if (node_list[i] == node_list[j])
				continue;
			
			if (j > i)
				continue;
			
			if (check_los(node_list[i]->pos, node_list[j]->pos)) {
				node_list[i]->links.push_back(j);
				node_list[j]->links.push_back(i);
			}
		}
	}
}

// finished.
bool Grit::check_los(const GPointType& pa, const GPointType& pb) {
	Generic::corelog() << "check_los: start " << pa.x << ", " << pa.y << ", "
												<< pb.x << ", " << pb.y << rn;
	
	if (gmag2(gsub2(pa, pb)) < 1e-3) //magic again.. maybe the floating point precision issue
	{
		printf("check_los: returning (directly) ...\n");
		return true;
	}
	
	for (auto poly : this->m_map->all_polys()) {
		for (std::size_t i = 0; i < poly->pts.size(); i++) {
			if (segments_cross(pa, pb, poly->pts[i], poly->pts[(i+1) % poly->pts.size()])) {
				return false;
			}
		}
	}
	
	Generic::corelog() << "check_los: returning true" << rn;
	return true;
}

// finished
// called by FindPath.
void Grit::link_node(GritNode& node, std::vector<GritNode *>& to_nodes) {
	// original code may need opt. here
	
	std::size_t node_idx = -1;
	for (std::size_t i = 0; i < to_nodes.size(); i++)
		if (to_nodes[i] == &node) {
			node_idx = i;
			break;
		}
		
	for (std::size_t i = 0; i < to_nodes.size(); i++) {
		// compare
		if (&node == to_nodes[i])
			continue;
		
		if (check_los(node.pos, to_nodes[i]->pos)) {
			node.links.push_back(i);
			to_nodes[i]->links.push_back(node_idx);
		}
	}
}

// finished. ( all_poly issue
bool Grit::pt_is_valid(const GPointType& pt) {
	
	const std::vector<GritPoly *> vect = this->m_map->all_polys();
	for (std::size_t i = 0; i < vect.size(); i++) {
		bool is_master = i < this->m_map->master_polys.size();
//		Generic::corelog() << "pt_is_valid: handling point, " << (is_master ? "master" : "not master") << rn;
		if (is_master ? (!pt_in_poly(pt, vect[i]->pts)) :
						pt_in_poly(pt, vect[i]->pts))
			return false;
	}
	
	return true;
}

// finished.
bool Grit::pt_is_concave(const std::vector<GPointType>& pts, std::size_t pt) {
	
	GPointType cur = pts[pt], next = pts[(pt+1) % pts.size()],
				prev = pts[(!pt) ? pts.size()-1 : pt-1];
	GPointType left { cur.x - prev.x, cur.y - prev.y },
				right { next.x - cur.x, next.y - cur.y };
	if ((left.x*right.y - left.y*right.x) > 0)
		Generic::corelog() << "concave point detected!" << rn;
	return (left.x*right.y - left.y*right.x) > 0;
}

// finished.
std::vector<GPointType> Grit::inflate_poly(const std::vector<GPointType>& pts, GUnitT dist) {
	std::vector<GPointType> ret;
	
	for (std::size_t i = 0; i < pts.size(); i++) {
		glm::vec2 ab = glm::normalize(GLVEC2(pts[(i+1) % pts.size()]) - GLVEC2(pts[i])),
			ac = glm::normalize(GLVEC2(pts[(!i) ? (pts.size()-1) : (i-1)]) - GLVEC2(pts[i]));
		glm::vec2 mid = glm::normalize(ab + ac);

		mid *= (float)(pt_is_concave(pts, i) ? dist : -dist);
		GPointType offseted = pts[i];
		offseted += GVEC2(mid);
		ret.push_back(offseted);
	}
	
	return ret;
}

// finished.
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

// finished.
bool Grit::pt_in_poly(const GPointType& pt, const std::vector<GPointType>& polypts) {
	GUnitT x_min = 1e+6; //magic
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

GPointType Grit::get_closer_edge_pt(const GPointType& pt) {
	std::vector<GPointType> possible_pts;
	GPointType closer { 0, 0 };
	float closer_vert_dist = 1e+12; // magic magic.
	
	for (auto poly : this->m_map->all_polys()) {
		std::vector<GPointType> infleated_pts = inflate_poly(poly->pts, 16);
		
		for (std::size_t i = 0; i < infleated_pts.size(); i++) {
			
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#ifdef CUBE_COMPILER_GCC
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif
			// TODO: a helper function, but still WIP here
			GPointType CUBEMUTEW_NOT_USED a = infleated_pts[i], b = infleated_pts[(i+1) % infleated_pts.size()];
			GPointType CUBEMUTEW_NOT_USED oa = poly->pts[i], ob = poly->pts[(i+1) % poly->pts.size()];
#pragma GCC diagnostic pop
			
			float dist = gmag2(gsub2(pt, infleated_pts[i]));
			if (dist < closer_vert_dist && pt_is_valid(infleated_pts[i])) {
				closer_vert_dist = dist;
				closer = infleated_pts[i];
			}
		}
	}
	
	possible_pts.push_back(closer);
	
	float closer_dist = 1e+12;
	std::size_t idx = 0;
	for (std::size_t i = 0; i < possible_pts.size(); i++) {
		float dist = gmag2(gsub2(pt, possible_pts[i]));
		if (dist < closer_dist) {
			closer_dist = dist;
			idx = i;
		}
	}
	return possible_pts[idx];
}