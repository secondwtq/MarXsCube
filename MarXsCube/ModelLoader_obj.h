//
//  ModelLoader_obj.h
//  MarXsCube
//
//  Created by SeconDatke on 1/21/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef __MarXsCube__ModelLoader_obj__
#define __MarXsCube__ModelLoader_obj__

#include "Common.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <array>

#include <glm/glm.hpp>

using FaceSpec = std::array<std::size_t, 3>;

class objfile;
class gl_vertarray;
class gl_vertarray_indexed;

void transfer_verts(gl_vertarray& dest, const objfile &src);

class gl_vertarray {
public:
	
	void clear();
	float *array() { return this->_data; }
	std::size_t count_face() { return this->_len; }
	std::size_t len() { return this->_len * 3 * 8; }
	void face(std::size_t len) { this->_len = len; }
	void init_with(std::size_t facecount) {
		this->clear();
		this->face(facecount);
		this->_data = new float [this->len()];
	}
	
	virtual ~gl_vertarray() { this->clear(); }
	
private:
	float *_data = nullptr;
	std::size_t _len = 0;
	
};

struct gl_vert_object {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 texcoord;
	glm::vec3 blendweight { 0 };
	glm::vec3 tile_index { 0 };
};

void transfer_verts_idx(gl_vertarray_indexed& dest, const objfile &src);

class gl_vertarray_indexed {
public:
	
	void clear() {
		this->m_len_vert = 0;
		delete [] this->m_vert_data;
		this->m_len_idx = 0;
		delete [] this->m_idx_data;
	}
	
	void init_with(std::size_t count_vert, std::size_t count_idx) {
		this->m_len_vert = count_vert;
		this->m_len_idx = count_idx;
		this->m_vert_data = new gl_vert_object[m_len_vert];
		this->m_idx_data = new GLIDX[m_len_idx];
	}
	
	gl_vert_object *verts() { return this->m_vert_data; }
	GLIDX *indexes() { return this->m_idx_data; }
	
	std::size_t count_vert() { return this->m_len_vert; }
	std::size_t count_idx() { return this->m_len_idx; }
	

private:
	gl_vert_object *m_vert_data = nullptr;
	GLIDX *m_idx_data = nullptr;
	
	std::size_t m_len_vert = 0;
	std::size_t m_len_idx = 0;
};

class objfile {
public:
	std::string filepath { "" }	;
	
	std::vector<glm::vec3> raw_verts, raw_uvcoords, raw_normals;
	std::vector<glm::i32vec3> raw_faces, raw_face_uvcoords;
	
	void parse();
	
	float **vert_array = nullptr;
};

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

void transfer_verts_tiler(tiler_dataarray& dest, const objfile&src);

struct tiler_drawcell {
	
	std::array<std::size_t, 6> vertices;
	bool seperated = false;
	
	tiler_drawcell(std::array<std::size_t, 6> pvertices) : vertices(pvertices) { }
	
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
	
	void update_vertorigin(std::size_t idx) {
		if (this->vert(idx).ismapped) {
			this->vert(this->vert(idx).origin_vert).position = this->vert(idx).position;
		}
	}
	
	std::size_t create_and_map(std::size_t from) {
		std::size_t idx = this->m_vert_data.size();
		this->m_vert_data.push_back({ });
		this->m_vert_data.at(idx).this_idx = static_cast<int>(idx);
		
		this->map_vert(from, idx);
		return idx;
	}
	
	VertObjectType &map_vert(std::size_t from, std::size_t to) {
		VertObjectType *vert_from = &this->m_vert_data.at(from);
		if (vert_from->ismapped) {
			vert_from = &this->m_vert_data.at(vert_from->origin_vert);
		}
		
		VertObjectType *vert_to = &this->m_vert_data.at(to);
		memcpy(vert_to, vert_from, sizeof(VertObjectType));
		
		vert_to->ismapped = true;
		vert_to->this_idx = static_cast<int>(to);
		vert_to->origin_vert = vert_from->this_idx;
		
		return *vert_to;
	}
	
	std::size_t find_nearest_cell(const glm::vec3 &pt) {
		std::size_t ret = 0;
		float current_distance = glm::distance(pt, m_cell_center[0]);
		
		for (std::size_t i = 0; i < this->m_cells.size(); i++) {
			if (glm::distance(pt, m_cell_center[i]) < current_distance) {
				ret = i;
				current_distance = glm::distance(pt, m_cell_center[i]);
			}
		}
		
		return ret;
	}
	
	std::size_t count_vert() { return this->m_vert_data.size(); }
	std::size_t count_idx() { return this->m_idx_data.size(); }
	
	void update_idx() {
		this->m_idx_data.clear();
		for (std::size_t i = 0; i < this->m_cells.size(); i++) {
			for (std::size_t j = 0; j < this->m_cells[i].vertices.size(); j++) {
				this->m_idx_data.push_back(static_cast<GLIDX>(this->m_cells[i].vertices[j]));
			}
		}
	}
	
	void sepreate_cell(std::size_t idx) {
		if (!m_cells[idx].seperated) {
			tiler_drawcell& cell = m_cells[idx];
			cell.seperated = true;
			std::array<std::size_t, CELL_EDGES> verts_to_map { cell.vertices[0], cell.vertices[1], cell.vertices[2], cell.vertices[4] };
			std::array<std::size_t, CELL_EDGES> verts_mapped { 0 };
			
			for (std::size_t i = 0; i < CELL_EDGES; i++)
				verts_mapped[i] = this->create_and_map(verts_to_map[i]);
			
			const std::size_t vertices_map[] = { 0, 1, 2, 2, 3, 0 };
			for (std::size_t i = 0; i < CELL_VERTS_TOTAL; i++)
				cell.vertices[i] = verts_mapped[vertices_map[i]];
		}
	}
	
private:
	std::vector<VertObjectType> m_vert_data;
	std::vector<GLIDX> m_idx_data;
	
	std::vector<tiler_drawcell> m_cells;
	std::vector<glm::vec3> m_cell_center;
};

#endif /* defined(__MarXsCube__ModelLoader_obj__) */
