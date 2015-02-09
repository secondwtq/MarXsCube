//
//  TeslaRenderingBasic.cpp
//  MarXsCube
//
//  Created by SeconDatke on 1/29/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "ModelLoader_obj.h"
#include "TeslaRenderingBasic.h"

#include <array>
#include <unordered_map>
#include <cstring>

void transfer_verts_tesla(tesla_dataarray& dest, const objfile& src) {
	std::unordered_map<std::size_t, std::size_t> texcoord_cache;
	
	for (std::size_t i = 0; i < src.raw_verts.size(); i++) {
		
		tesla_dataarray::VertObjectType vert;
		
		vert.position = src.raw_verts[i];
		if (src.raw_normals.size() > i)
			vert.normal = src.raw_normals[i];
		
		if (texcoord_cache.find(i) != texcoord_cache.end()) {
			vert.texcoord = src.raw_uvcoords[texcoord_cache.at(i)];
		} else {
			for (int j = 0; j < src.raw_faces.size(); j++) {
				for (int k = 0; k < 3; k++) {
					auto vert_idx = src.raw_faces[j][k];
					texcoord_cache[vert_idx] = src.raw_face_uvcoords[j][k];
					if (vert_idx == i) {
						vert.texcoord = src.raw_uvcoords[src.raw_face_uvcoords[j][k]];
						break;
					}
				}
			}
		}
		
		vert.this_idx = static_cast<int>(i);
		dest.vec_verts().push_back(vert);
	}
	
	std::array<std::size_t, CELL_VERTS_TOTAL> cellvert_buffer;
	for (std::size_t i = 0; i < src.raw_faces.size(); i++) {
		glm::i32vec3 current_face = src.raw_faces[i];
		for (int j = 0; j < 3; j++)
			dest.vec_indexes().push_back(current_face[j]);
		
		if (i % 2 == 1) {
			for (int j = 0; j < 3; j++)
				cellvert_buffer[j+3] = current_face[j];
			
			dest.vec_cells().push_back({ cellvert_buffer, static_cast<int>(dest.vec_cells().size()) });
			
			glm::vec3 center { 0 };
			center = dest.vec_verts()[cellvert_buffer[0]].position + dest.vec_verts()[cellvert_buffer[1]].position +
			dest.vec_verts()[cellvert_buffer[2]].position + dest.vec_verts()[cellvert_buffer[4]].position;
			center /= 4.0f;
			dest.vec_centers().push_back(center);
		} else
			for (int j = 0; j < 3; j++)
				cellvert_buffer[j] = current_face[j];
	}
}

void tesla_dataarray::sepreate_cell(std::size_t idx) {
	if (!m_cells[idx].seperated) {
		// set as seperated
		tesla_drawcell& cell = m_cells[idx];
		cell.seperated = true;
		
		// map vertices
		std::array<std::size_t, CELL_EDGES> verts_to_map {{ cell.vertices[0], cell.vertices[1], cell.vertices[2], cell.vertices[4] }};
		std::array<std::size_t, CELL_EDGES> verts_mapped {{ 0 }}; // it's very strange that
			// clang would complain about single brace with -Wall, and double brack looks ugly
		for (std::size_t i = 0; i < CELL_EDGES; i++)
			verts_mapped[i] = this->create_and_map(verts_to_map[i]);
		
		// update vertex indexes in original cell
		const std::size_t vertices_map[] = { 0, 1, 2, 2, 3, 0 };
		for (std::size_t i = 0; i < CELL_VERTS_TOTAL; i++)
			cell.vertices[i] = verts_mapped[vertices_map[i]];
	}
}

std::size_t tesla_dataarray::find_nearest_cell(const glm::vec3 &pt) {
	std::size_t ret = 0;
	float current_distance = glm::distance(pt, m_cell_center[0]);
	
	for (std::size_t i = 0; i < this->m_cells.size(); i++)
		if (glm::distance(pt, m_cell_center[i]) < current_distance) {
			ret = i;
			current_distance = glm::distance(pt, m_cell_center[i]);
		}
	
	return ret;
}

tesla_dataarray::VertObjectType &tesla_dataarray::map_vert(std::size_t from, std::size_t to) {
	VertObjectType *vert_from = &this->m_vert_data.at(from);
	if (vert_from->ismapped)
		vert_from = &this->m_vert_data.at(vert_from->origin_vert);
	VertObjectType *vert_to = &this->m_vert_data.at(to);
	
	// copy data from source vertex
	memcpy(vert_to, vert_from, sizeof(VertObjectType));
	
	// set mapping metadata
	vert_to->ismapped = true;
	vert_to->this_idx = static_cast<int>(to);
	vert_to->origin_vert = vert_from->this_idx;
	
	return *vert_to;
}