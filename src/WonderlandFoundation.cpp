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

namespace Wonderland {

	namespace Foundation {

void set_texture_and_blend(TeslaObject *chunk, std::size_t index, const CoordStruct& position) {
	tesla_dataarray *data = chunk->get_data();
	
	std::size_t nearest_cellidx = data->find_nearest_cell({ position.y, position.x, 0 });
	glm::vec3 nearest_cellcenter = data->vec_centers()[nearest_cellidx];
	
	data->sepreate_cell(nearest_cellidx);
	
	std::array<std::size_t, 4> vert_idx_to_iter {{ 0, 1, 2, 4 }};
	for (std::size_t i : vert_idx_to_iter) {
		data->vert(data->cell(nearest_cellidx).vertices[i]).tile_indexes.x = index;
		data->vert(data->cell(nearest_cellidx).vertices[i]).tile_indexes.y = index;
		data->update_vertorigin(data->cell(nearest_cellidx).vertices[i]);
		data->update_map_of(data->vert(data->cell(nearest_cellidx).vertices[i]).get_origin());
	}
	//	verts_data.vert(verts_data.cell(nearest_cellidx).vertices[0]).blendweights.x = 1.0;
	
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
		for (std::size_t i : vert_idx_to_iter)
			data->vert(cell->vertices[i]).tile_indexes.y = index;
	}
	
	data->vert(leftcell.vertices[1]).blendweights.x = 1.0;
	data->vert(leftcell.vertices[2]).blendweights.x = 1.0;
	
	data->vert(rightcell.vertices[0]).blendweights.x = 1.0;
	data->vert(rightcell.vertices[4]).blendweights.x = 1.0;
	
	data->vert(topcell.vertices[2]).blendweights.x = 1.0;
	data->vert(topcell.vertices[4]).blendweights.x = 1.0;
	
	data->vert(bottomcell.vertices[0]).blendweights.x = 1.0;
	data->vert(bottomcell.vertices[1]).blendweights.x = 1.0;
	
	data->vert(lefttopcell.vertices[2]).blendweights.x = 1.0;
	data->vert(rightbottomcell.vertices[0]).blendweights.x = 1.0;
	data->vert(leftbottomcell.vertices[1]).blendweights.x = 1.0;
	data->vert(righttopcell.vertices[4]).blendweights.x = 1.0;
	
	data->update_idx();
	
	Acheron::Silcon.pause();
	chunk->buffer_vert()->use();
	glBufferSubData(GL_ARRAY_BUFFER, 0, data->count_vert() * sizeof(tesla_dataarray::VertObjectType), data->verts());
	GLFoundation::unbind(*chunk->buffer_vert());
	Acheron::Silcon.invoke();
	
	Acheron::Silcon.pause();
	chunk->buffer_idx()->use();
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, data->count_idx() * sizeof(GLIDX), data->indexes());
	GLFoundation::unbind(*chunk->buffer_idx());
	Acheron::Silcon.invoke();
}

	}
	
}