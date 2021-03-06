//
//  TeslaRenderingBasic.h
//  MarXsCube
//
//  Created by SeconDatke on 1/29/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef __MarXsCube__TeslaRenderingBasic__
#define __MarXsCube__TeslaRenderingBasic__

#include "ModelLoader_obj.h"

#include <glm/glm.hpp>
#include <array>

#define CELL_EDGES 4
#define CELL_VERTS_TOTAL 6
#define CELL_FACE_DISTANCE 32

class tesla_dataarray;

struct tesla_vert {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 texcoord;
	glm::vec3 blendweights { 0 };
	glm::vec3 tile_indexes { 0 };
	
	bool ismapped = false;
	int this_idx = -1;
	int origin_vert = -1;
	
	std::size_t get_origin() {
		if (this->ismapped) return this->origin_vert;
		else return this->this_idx;
	}
};

void transfer_verts_tesla(tesla_dataarray& dest, const objfile& src);

struct tesla_drawcell {
	
	tesla_dataarray *parent = nullptr;
	std::array<std::size_t, CELL_VERTS_TOTAL> vertices;
	bool seperated = false;
	int this_idx = -1;
	std::size_t tileidx = 0;
	std::size_t secondary_idx = 0;
	std::size_t third_idx = 0;
	bool t = false;
	
	void set_tileid(std::size_t idx);
	void set_secondid(std::size_t idx);
	void set_thirdid(std::size_t idx);
	void override_tileid(std::size_t idx);
	std::size_t get_tileid() { return this->tileidx; }
	std::size_t get_secondid() { return this->secondary_idx; }
	std::size_t get_thirdid() { return this->third_idx; }
	
	tesla_vert *vert(std::size_t id);
	
	tesla_drawcell *left();
	tesla_drawcell *top();
	tesla_drawcell *right();
	tesla_drawcell *bottom();
	
	tesla_drawcell *lefttop();
	tesla_drawcell *leftbottom();
	tesla_drawcell *righttop();
	tesla_drawcell *rightbottom();
	
	tesla_drawcell(tesla_dataarray *p)
											: parent(p) { }
	
	tesla_drawcell(tesla_dataarray *p, std::array<std::size_t, CELL_VERTS_TOTAL> pvertices, int idx)
											: parent(p), vertices(pvertices), this_idx(idx) { }
	
};

// NOTICE: the helpers in this class is not a good design
//		and they are not thread safe.
class tesla_dataarray {
public:
	
	typedef tesla_vert VertObjectType;
	
	std::vector<VertObjectType>& vec_verts() { return this->m_vert_data; }
	std::vector<GLIDX>& vec_indexes() { return this->m_idx_data; }
	std::vector<tesla_drawcell>& vec_cells() { return this->m_cells; }
	std::vector<glm::vec3>& vec_centers() { return this->m_cell_center; }
	
	tesla_drawcell& cell(std::size_t idx) { return this->m_cells.at(idx); }
	VertObjectType& vert(std::size_t idx) { return this->m_vert_data.at(idx); }
	const glm::vec3& center(std::size_t idx) { return this->m_cell_center.at(idx); }
	
	VertObjectType *verts() { return this->m_vert_data.data(); }
	GLIDX *indexes() { return this->m_idx_data.data(); }
	
	std::size_t create_and_map(std::size_t from) {
		std::size_t idx = this->m_vert_data.size();
		this->m_vert_data.push_back({ });
		this->m_vert_data.at(idx).this_idx = static_cast<int>(idx);
		
		this->map_vert(from, idx);
		return idx;
	}
	
	VertObjectType &map_vert(std::size_t from, std::size_t to);
	
	std::size_t find_nearest_cell(const glm::vec3 &pt);
	
	std::size_t count_vert() { return this->m_vert_data.size(); }
	std::size_t count_idx() { return this->m_idx_data.size(); }
	
	void update_vertorigin(std::size_t idx) {
		if (this->vert(idx).ismapped) {
			const VertObjectType& vert = this->vert(idx);
			this->vert(vert.origin_vert).position = vert.position;
		}
	}
	
	void update_map_of(std::size_t idx) {
		if (!this->vert(idx).ismapped) {
			const VertObjectType& vert = this->vert(idx);
			for (std::size_t i = 0; i < this->count_vert(); i++) {
				if (this->vert(i).origin_vert == vert.this_idx) {
					this->vert(i).position = vert.position;
				}
			}
		}
	}
	
	void update_idx() {
		this->m_idx_data.clear();
		for (std::size_t i = 0; i < this->m_cells.size(); i++)
			for (std::size_t j = 0; j < this->m_cells[i].vertices.size(); j++)
				this->m_idx_data.push_back(static_cast<GLIDX>(this->m_cells[i].vertices[j]));
	}
	
	void sepreate_cell(std::size_t idx);
	
	int find_cell(std::size_t idx, const glm::vec3& offset, float threshold = 8.0f) {
		glm::vec3 org_center = this->m_cell_center.at(idx);
		
		// you'll need to swap x and y coord, WHY?
		glm::vec3 center_tofind = org_center + glm::vec3(offset.y, offset.x, 0);
		
		for (std::size_t i = 0; i < this->m_cell_center.size(); i++)
			if (glm::distance(center_tofind, this->center(i)) <= threshold)
				return static_cast<int>(i);
		return -1;
	}
	
	int cell_left(std::size_t idx) { return this->find_cell(idx, { -CELL_FACE_DISTANCE, 0, 0 }); }
	int cell_right(std::size_t idx) { return this->find_cell(idx, { CELL_FACE_DISTANCE, 0, 0 }); }
	int cell_top(std::size_t idx) { return this->find_cell(idx, { 0, CELL_FACE_DISTANCE, 0 }); }
	int cell_bottom(std::size_t idx) { return this->find_cell(idx, { 0, -CELL_FACE_DISTANCE, 0 }); }
	
	int cell_lefttop(std::size_t idx) { return this->find_cell(idx, { -CELL_FACE_DISTANCE, CELL_FACE_DISTANCE, 0 }); }
	int cell_leftbottom(std::size_t idx) { return this->find_cell(idx, { -CELL_FACE_DISTANCE, -CELL_FACE_DISTANCE, 0 }); }
	int cell_righttop(std::size_t idx) { return this->find_cell(idx, { CELL_FACE_DISTANCE, CELL_FACE_DISTANCE, 0 }); }
	int cell_rightbottom(std::size_t idx) { return this->find_cell(idx, { CELL_FACE_DISTANCE, -CELL_FACE_DISTANCE, 0 }); }
	
private:
	std::vector<VertObjectType> m_vert_data;
	std::vector<GLIDX> m_idx_data;
	
	std::vector<tesla_drawcell> m_cells;
	std::vector<glm::vec3> m_cell_center;
};

#endif /* defined(__MarXsCube__TeslaRenderingBasic__) */
