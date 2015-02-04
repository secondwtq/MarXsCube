//
//  TilerRenderingBasic.h
//  MarXsCube
//
//  Created by SeconDatke on 1/29/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef __MarXsCube__TilerRenderingBasic__
#define __MarXsCube__TilerRenderingBasic__

#include "ModelLoader_obj.h"

#include <glm/glm.hpp>
#include <array>

#define CELL_EDGES 4
#define CELL_VERTS_TOTAL 6
#define CELL_FACE_DISTANCE 32

class tiler_dataarray;

struct tiler_vert {
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

void transfer_verts_tiler(tiler_dataarray& dest, const objfile& src);

struct tiler_drawcell {
	
	std::array<std::size_t, CELL_VERTS_TOTAL> vertices;
	bool seperated = false;
	int this_idx = -1;
	
	tiler_drawcell(std::array<std::size_t, CELL_VERTS_TOTAL> pvertices, int idx)
											: vertices(pvertices), this_idx(idx) { }
	
};

// NOTICE: the helpers in this class is not a good design
//		and they are not thread safe.
class tiler_dataarray {
public:
	
	typedef tiler_vert VertObjectType;
	
	std::vector<VertObjectType>& vec_verts() { return this->m_vert_data; }
	std::vector<GLIDX>& vec_indexes() { return this->m_idx_data; }
	std::vector<tiler_drawcell>& vec_cells() { return this->m_cells; }
	std::vector<glm::vec3>& vec_centers() { return this->m_cell_center; }
	
	tiler_drawcell& cell(std::size_t idx) { return this->m_cells.at(idx); }
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
	
	std::size_t find_cell(std::size_t idx, const glm::vec3& offset, float threshold = 8.0f) {
		glm::vec3 org_center = this->m_cell_center.at(idx);
		
		// you'll need to swap x and y coord, WHY?
		glm::vec3 center_tofind = org_center + glm::vec3(offset.y, offset.x, 0);
		
		for (std::size_t i = 0; i < this->m_cell_center.size(); i++) {
			if (glm::distance(center_tofind, this->center(i)) <= threshold) {
				return i;
			}
		}
		return -1;
	}
	
	std::size_t cell_left(std::size_t idx) { return this->find_cell(idx, { -CELL_FACE_DISTANCE, 0, 0 }); }
	std::size_t cell_right(std::size_t idx) { return this->find_cell(idx, { CELL_FACE_DISTANCE, 0, 0 }); }
	std::size_t cell_top(std::size_t idx) { return this->find_cell(idx, { 0, CELL_FACE_DISTANCE, 0 }); }
	std::size_t cell_bottom(std::size_t idx) { return this->find_cell(idx, { 0, -CELL_FACE_DISTANCE, 0 }); }
	
	std::size_t cell_lefttop(std::size_t idx) { return this->find_cell(idx, { -CELL_FACE_DISTANCE, CELL_FACE_DISTANCE, 0 }); }
	std::size_t cell_leftbottom(std::size_t idx) { return this->find_cell(idx, { -CELL_FACE_DISTANCE, -CELL_FACE_DISTANCE, 0 }); }
	std::size_t cell_righttop(std::size_t idx) { return this->find_cell(idx, { CELL_FACE_DISTANCE, CELL_FACE_DISTANCE, 0 }); }
	std::size_t cell_rightbottom(std::size_t idx) { return this->find_cell(idx, { CELL_FACE_DISTANCE, -CELL_FACE_DISTANCE, 0 }); }
	
private:
	std::vector<VertObjectType> m_vert_data;
	std::vector<GLIDX> m_idx_data;
	
	std::vector<tiler_drawcell> m_cells;
	std::vector<glm::vec3> m_cell_center;
};

#endif /* defined(__MarXsCube__TilerRenderingBasic__) */
