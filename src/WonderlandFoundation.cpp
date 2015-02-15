//
//  WonderlandFoundation.cpp
//  MarXsCube
//
//  Created by SeconDatke on 2/5/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "Common.h"
#include "Generic.h"
#include "GLFoundation.h"
#include "SilconThread.h"
#include "TeslaObject.h"
#include "WonderlandFoundation.h"

#include <cmath>
#include <array>
#include <functional>

namespace Wonderland {

	namespace Foundation {
		
		void check_cell(TeslaObject *chunk, std::size_t index) {
			tesla_dataarray *data = chunk->get_data();
			
			tesla_drawcell &cell = data->cell(index);
			float initial = data->vert(cell.vertices[0]).blendweights.x;
			if (initial < 1e-4) return;
			
			for (std::size_t i = 0; i < 6; i++)
				if (data->vert(cell.vertices[i]).blendweights.x != initial)
					return;
			
			cell.override_tileid(cell.vert(0)->tile_indexes.y);
		}

void set_texture_and_blend(TeslaObject *chunk, std::size_t index, const CoordStruct& position) {
	tesla_dataarray *data = chunk->get_data();
	
	std::size_t nearest_cellidx = data->find_nearest_cell({ position.y, position.x, 0 });
	
	data->sepreate_cell(nearest_cellidx);
	
	if (data->vert(data->cell(nearest_cellidx).vertices[0]).tile_indexes.x == index) return;
	std::array<std::size_t, 4> vert_idx_to_iter {{ 0, 1, 2, 4 }};
	for (std::size_t i : vert_idx_to_iter) {
		data->cell(nearest_cellidx).override_tileid(index);
		data->update_vertorigin(data->cell(nearest_cellidx).vertices[i]);
		data->update_map_of(data->vert(data->cell(nearest_cellidx).vertices[i]).get_origin());
	}
	
	tesla_drawcell &leftcell = data->cell(data->cell_left(nearest_cellidx));
	tesla_drawcell &rightcell = data->cell(data->cell_right(nearest_cellidx));
	tesla_drawcell &topcell = data->cell(data->cell_top(nearest_cellidx));
	tesla_drawcell &bottomcell = data->cell(data->cell_bottom(nearest_cellidx));
	tesla_drawcell &lefttopcell = data->cell(data->cell_lefttop(nearest_cellidx));
	tesla_drawcell &leftbottomcell = data->cell(data->cell_leftbottom(nearest_cellidx));
	tesla_drawcell &righttopcell = data->cell(data->cell_righttop(nearest_cellidx));
	tesla_drawcell &rightbottomcell = data->cell(data->cell_rightbottom(nearest_cellidx));
	
	std::array<tesla_drawcell *, 8> edge_cells {{ &leftcell, &rightcell, &topcell, &bottomcell,
		&lefttopcell, &leftbottomcell, &righttopcell, &rightbottomcell }};
	
	for (auto cell : edge_cells) {
		data->sepreate_cell(cell->this_idx);
		cell->set_secondid(index);
	}
	data->update_idx();
	
	std::array<float, 6> left_blendweights = {{ 0.0, 1.0, 1.0, 1.0, 0.0, 0.0 }};
	std::array<float, 6> right_blendweights = {{ 1.0, 0.0, 0.0, 0.0, 1.0, 1.0 }};
	std::array<float, 6> top_blendweights = {{ 0.0, 0.0, 1.0, 1.0, 1.0, 0.0 }};
	std::array<float, 6> bottom_blendweights = {{ 1.0, 1.0, 0.0, 0.0, 0.0, 1.0 }};
	std::array<float, 6> lefttop_blendweights = {{ 0.0, 0.0, 1.0, 1.0, 0.0, 0.0 }};
	std::array<float, 6> rightbottom_blendweights = {{ 1.0, 0.0, 0.0, 0.0, 0.0, 1.0 }};
	std::array<float, 6> leftbottom_blendweights = {{ 0.0, 1.0, 0.0, 0.0, 0.0, 0.0 }};
	std::array<float, 6> righttop_blendweights = {{ 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 }};
	
	std::function<void (tesla_drawcell& cell, const std::array<float, 6>& weights)> set_blendweights = [data, nearest_cellidx] (tesla_drawcell& cell, const std::array<float, 6>& weights) {
		for (std::size_t i = 0; i < 6; i++) {
			if (data->vert(cell.vertices[i]).tile_indexes.y == data->vert(data->cell(nearest_cellidx).vertices[i]).tile_indexes.x) {
				data->vert(cell.vertices[i]).blendweights.x = std::min(data->vert(cell.vertices[i]).blendweights.x+weights[i], 1.0f);
			}
			if (data->vert(cell.vertices[i]).tile_indexes.x == data->vert(cell.vertices[i]).tile_indexes.y) {
				data->vert(cell.vertices[i]).blendweights.x = 0; }
//			if (data->vert(cell.vertices[i]).tile_indexes.x != data->vert(data->cell(nearest_cellidx).vertices[i]).tile_indexes.x)
		}
	};
	
	set_blendweights(leftcell, left_blendweights);
	set_blendweights(rightcell, right_blendweights);
	set_blendweights(topcell, top_blendweights);
	set_blendweights(bottomcell, bottom_blendweights);
	set_blendweights(lefttopcell, lefttop_blendweights);
	set_blendweights(rightbottomcell, rightbottom_blendweights);
	set_blendweights(leftbottomcell, leftbottom_blendweights);
	set_blendweights(righttopcell, righttop_blendweights);
	
	check_cell(chunk, nearest_cellidx);
	check_cell(chunk, leftcell.this_idx);
	check_cell(chunk, rightcell.this_idx);
	check_cell(chunk, topcell.this_idx);
	check_cell(chunk, bottomcell.this_idx);
	check_cell(chunk, lefttopcell.this_idx);
	check_cell(chunk, rightbottomcell.this_idx);
	check_cell(chunk, leftbottomcell.this_idx);
	check_cell(chunk, righttopcell.this_idx);
	
}
		
void buffer_update(TeslaObject *chunk) {
	tesla_dataarray *data = chunk->get_data();
	Acheron::Silcon.pause();
	
	chunk->buffer_vert()->use();
	glBufferSubData(GL_ARRAY_BUFFER, 0, data->count_vert() * sizeof(tesla_dataarray::VertObjectType), data->verts());
	GLFoundation::unbind(*chunk->buffer_vert());
	chunk->buffer_idx()->use();
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, data->count_idx() * sizeof(GLIDX), data->indexes());
	GLFoundation::unbind(*chunk->buffer_idx());
	
	Acheron::Silcon.invoke();
}

	}
	
}