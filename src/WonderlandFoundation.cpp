//
//  WonderlandFoundation.cpp
//  MarXsCube
//
//  Created by SeconDatke on 2/5/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "Generic.h"
#include "Transform.h"
#include "SilconThread.h"
#include "WonderlandFoundation.h"

#include <cmath>

void tiler_array_test() {
/*
	CoordStruct current_pos = obsTransform::GetWorldPos(Generic::Session()->MousePosData.pos);
	
	std::size_t nearest_cellidx = verts_data.find_nearest_cell({ current_pos.y, current_pos.x, 0 });
	glm::vec3 nearest_cellcenter = verts_data.vec_centers()[nearest_cellidx];
	
	verts_data.sepreate_cell(nearest_cellidx);
	
	std::array<std::size_t, 4> vert_idx_to_iter { 0, 1, 2, 4 };
	for (std::size_t i : vert_idx_to_iter) {
		//		verts_data.vert(verts_data.cell(nearest_cellidx).vertices[i]).position.z += 16.0;
		verts_data.vert(verts_data.cell(nearest_cellidx).vertices[i]).tile_indexes.x = 3.0;
		verts_data.vert(verts_data.cell(nearest_cellidx).vertices[i]).tile_indexes.y = 3.0;
		verts_data.update_vertorigin(verts_data.cell(nearest_cellidx).vertices[i]);
		verts_data.update_map_of(verts_data.vert(verts_data.cell(nearest_cellidx).vertices[i]).get_origin());
	}
	//	verts_data.vert(verts_data.cell(nearest_cellidx).vertices[0]).blendweights.x = 1.0;
	
	tiler_drawcell &leftcell = verts_data.cell(verts_data.cell_left(nearest_cellidx));
	tiler_drawcell &rightcell = verts_data.cell(verts_data.cell_right(nearest_cellidx));
	tiler_drawcell &topcell = verts_data.cell(verts_data.cell_top(nearest_cellidx));
	tiler_drawcell &bottomcell = verts_data.cell(verts_data.cell_bottom(nearest_cellidx));
	tiler_drawcell &lefttopcell = verts_data.cell(verts_data.cell_lefttop(nearest_cellidx));
	tiler_drawcell &leftbottomcell = verts_data.cell(verts_data.cell_leftbottom(nearest_cellidx));
	tiler_drawcell &righttopcell = verts_data.cell(verts_data.cell_righttop(nearest_cellidx));
	tiler_drawcell &rightbottomcell = verts_data.cell(verts_data.cell_rightbottom(nearest_cellidx));
	
	std::array<tiler_drawcell *, 8> edge_cells { &leftcell, &rightcell, &topcell, &bottomcell,
		&lefttopcell, &leftbottomcell, &righttopcell, &rightbottomcell };
	
	for (auto cell : edge_cells) {
		verts_data.sepreate_cell(cell->this_idx);
		for (std::size_t i : vert_idx_to_iter)
			verts_data.vert(cell->vertices[i]).tile_indexes.y = 3.0;
	}
	
	verts_data.vert(leftcell.vertices[1]).blendweights.x = 1.0;
	verts_data.vert(leftcell.vertices[2]).blendweights.x = 1.0;
	
	verts_data.vert(rightcell.vertices[0]).blendweights.x = 1.0;
	verts_data.vert(rightcell.vertices[4]).blendweights.x = 1.0;
	
	verts_data.vert(topcell.vertices[2]).blendweights.x = 1.0;
	verts_data.vert(topcell.vertices[4]).blendweights.x = 1.0;
	
	verts_data.vert(bottomcell.vertices[0]).blendweights.x = 1.0;
	verts_data.vert(bottomcell.vertices[1]).blendweights.x = 1.0;
	
	verts_data.vert(lefttopcell.vertices[2]).blendweights.x = 1.0;
	verts_data.vert(rightbottomcell.vertices[0]).blendweights.x = 1.0;
	verts_data.vert(leftbottomcell.vertices[1]).blendweights.x = 1.0;
	verts_data.vert(righttopcell.vertices[4]).blendweights.x = 1.0;
	
	verts_data.update_idx();
	
	Acheron::Silcon.pause();
	glBindBuffer(GL_ARRAY_BUFFER, vert_buf_new);
	glBufferSubData(GL_ARRAY_BUFFER, 0, verts_data.count_vert() * sizeof(tiler_dataarray::VertObjectType), verts_data.verts());
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	Acheron::Silcon.invoke();
	
	Acheron::Silcon.pause();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idx_buf);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, verts_data.count_idx() * sizeof(GLIDX), verts_data.indexes());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	Acheron::Silcon.invoke();
 */
}