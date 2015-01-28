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

class tiler_dataarray;

struct tiler_vert {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 texcoord;
	glm::vec3 blendweight { 0 };
	glm::vec3 tile_index { 0 };
	
	bool ismapped = false;
	int this_idx = -1;
	int origin_vert = -1;
};

void transfer_verts_tiler(tiler_dataarray& dest, const objfile& src);

struct tiler_drawcell {
	
	std::array<std::size_t, CELL_VERTS_TOTAL> vertices;
	bool seperated = false;
	
	tiler_drawcell(std::array<std::size_t, CELL_VERTS_TOTAL> pvertices) : vertices(pvertices) { }
	
};

class tiler_dataarray {
public:
	
	typedef tiler_vert VertObjectType;
	
	std::vector<VertObjectType>& vec_verts() { return this->m_vert_data; }
	std::vector<GLIDX>& vec_indexes() { return this->m_idx_data; }
	std::vector<tiler_drawcell>& vec_cells() { return this->m_cells; }
	std::vector<glm::vec3>& vec_centers() { return this->m_cell_center; }
	
	tiler_drawcell &cell(std::size_t idx) { return this->m_cells.at(idx); }
	VertObjectType &vert(std::size_t idx) { return this->m_vert_data.at(idx); }
	
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
		if (this->vert(idx).ismapped)
			this->vert(this->vert(idx).origin_vert).position = this->vert(idx).position;
	}
	
	void update_idx() {
		this->m_idx_data.clear();
		for (std::size_t i = 0; i < this->m_cells.size(); i++)
			for (std::size_t j = 0; j < this->m_cells[i].vertices.size(); j++)
				this->m_idx_data.push_back(static_cast<GLIDX>(this->m_cells[i].vertices[j]));
	}
	
	void sepreate_cell(std::size_t idx);
	
private:
	std::vector<VertObjectType> m_vert_data;
	std::vector<GLIDX> m_idx_data;
	
	std::vector<tiler_drawcell> m_cells;
	std::vector<glm::vec3> m_cell_center;
};

#endif /* defined(__MarXsCube__TilerRenderingBasic__) */
