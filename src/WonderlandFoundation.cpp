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
#include "WonderlandCommon.h"
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

void set_texture(TeslaObject *chunk, std::size_t index, const CoordStruct& position) {
	tesla_dataarray *data = chunk->get_data();
	std::size_t nearest_cellidx = data->find_nearest_cell({ position.y, position.x, 0 });
	data->sepreate_cell(nearest_cellidx);
	
	data->cell(nearest_cellidx).override_tileid(index);
	std::array<std::size_t, 4> vert_idx_to_iter {{ 0, 1, 2, 4 }};
	
	for (std::size_t i : vert_idx_to_iter) {
		data->update_vertorigin(data->cell(nearest_cellidx).vertices[i]);
		data->update_map_of(data->vert(data->cell(nearest_cellidx).vertices[i]).get_origin());
	}
	
	data->update_idx();
}

void foo_cell_batchblend(TeslaObject *chunk, tesla_drawcell *cell) {
	if (!cell->t) {
//		printf("blending cells\n");
		cell_fix_blend(chunk, cell->this_idx);
		cell->t = true;
		
		std::array <tesla_drawcell *, 4> cells {{ cell->top(), cell->bottom(), cell->left(), cell->right() }};
		for (auto cell_m : cells) {
			if (cell_m) {
				if (cell_m->get_tileid() == cell->get_tileid())
					foo_cell_batchblend(chunk, cell_m);
			}
		}
	}
}

void blend_cells_batch(TeslaObject *chunk, const CoordStruct& position) {
	tesla_dataarray *data = chunk->get_data();
	
	std::size_t nearest_cellidx = data->find_nearest_cell({ position.y, position.x, 0 });
	
	foo_cell_batchblend(chunk, &data->cell(nearest_cellidx));
	
	for (tesla_drawcell& cell : chunk->get_data()->vec_cells()) {
		cell.t = false; }
}

void cell_fix_blend(TeslaObject *chunk, std::size_t cell_idx) {
	tesla_dataarray *data = chunk->get_data();
	tesla_drawcell& cell = data->cell(cell_idx);
	std::size_t masterid = cell.get_tileid();
	
	std::array<tesla_drawcell *, 8> edge_cells {{ cell.top(), cell.bottom(), cell.left(), cell.right(),
		cell.righttop(), cell.rightbottom(), cell.lefttop(), cell.leftbottom() }};
	
	for (auto cell : edge_cells) {
		if (cell) {
			std::size_t org_primary = cell->get_tileid(), org_second = cell->get_secondid();
			data->sepreate_cell(cell->this_idx);
			cell->set_tileid(org_primary), cell->set_secondid(org_second);
		}
	}
	data->update_idx();
	
	std::array<std::size_t, 4> vert_idx_to_iter {{ 0, 1, 2, 4 }};
	for (auto cell : edge_cells) {
		if (cell && cell->get_secondid() != cell->get_tileid()) {
			if (cell->get_secondid() == cell->get_thirdid()) {
				for (std::size_t i : vert_idx_to_iter) {
					cell->vert(i)->blendweights.x = std::min(cell->vert(i)->blendweights.x+cell->vert(i)->blendweights.y, 1.0f);
					cell->vert(i)->blendweights.y = 0.0f;
					cell->vert(i)->tile_indexes.z = cell->vert(i)->tile_indexes.x;
				}
			}
		}
	}

	std::function<bool (tesla_drawcell *)> match = [masterid] (tesla_drawcell *cell) {
		return !cell || masterid == cell->get_tileid(); };
	
	std::function<bool (tesla_drawcell *)> setid = [masterid] (tesla_drawcell *cell) {
		if (cell->get_secondid() != cell->get_tileid()) {
			if (cell->get_secondid() != masterid) {
				cell->set_thirdid(masterid);
			} else {
				cell->set_secondid(masterid);
				return true;
			}
		} else {
			cell->set_secondid(masterid);
		}
		return false;
	};
	
	if (!match(cell.bottom())) {
		bool t = setid(cell.bottom());
		apply_blend(chunk, cell.bottom()->this_idx, BOTTOM, masterid, t);
		
		if ((!match(cell.rightbottom())) && (!match(cell.leftbottom()))) { }
		else if (match(cell.rightbottom())) {
			apply_blend(chunk, cell.bottom()->this_idx, LEFT, masterid, true);
		} else {
			apply_blend(chunk, cell.bottom()->this_idx, RIGHT, masterid, true); }
	}
	
	if (!match(cell.top())) {
		bool t = setid(cell.top());
		apply_blend(chunk, cell.top()->this_idx, TOP, masterid, t);
		
		if ((!match(cell.lefttop())) && (!match(cell.righttop()))) { }
		else if (match(cell.righttop())) {
			apply_blend(chunk, cell.top()->this_idx, LEFT, masterid, true);
		} else {
			apply_blend(chunk, cell.top()->this_idx, RIGHT, masterid, true); }
	}
	
	if (!match(cell.left())) {
		bool t = setid(cell.left());
		apply_blend(chunk, cell.left()->this_idx, LEFT, masterid, t);
		
		if ((!match(cell.lefttop())) && (!match(cell.leftbottom()))) { }
		else if (match(cell.lefttop())) {
			apply_blend(chunk, cell.left()->this_idx, BOTTOM, masterid, true);
		} else {
			apply_blend(chunk, cell.left()->this_idx, TOP, masterid, true); }
	}
	
	if (!match(cell.right())) {
		bool t = setid(cell.right());
		apply_blend(chunk, cell.right()->this_idx, RIGHT, masterid, t);
		
		if ((!match(cell.righttop())) && (!match(cell.rightbottom()))) { }
		else if (match(cell.righttop())) {
			apply_blend(chunk, cell.right()->this_idx, BOTTOM, masterid, true);
		} else {
			apply_blend(chunk, cell.right()->this_idx, TOP, masterid, true); }
	}
	
	if (!match(cell.leftbottom())) {
		bool t = setid(cell.leftbottom());
		if (!match(cell.left()) && (!match(cell.bottom()))) {
			apply_blend(chunk, cell.leftbottom()->this_idx, LEFTBOTTOM, masterid, t);
		} else if ((match(cell.left())) && (match(cell.bottom()))) {
			apply_blend(chunk, cell.leftbottom()->this_idx, LEFT, masterid);
			apply_blend(chunk, cell.leftbottom()->this_idx, BOTTOM, masterid, true);
		}
	}
	
	if (!match(cell.rightbottom())) {
		bool t = setid(cell.rightbottom());
		if (!match(cell.right()) && (!match(cell.bottom()))) {
			apply_blend(chunk, cell.rightbottom()->this_idx, RIGHTBOTTOM, masterid, t);
		} else if ((match(cell.right())) && (match(cell.bottom()))) {
			apply_blend(chunk, cell.rightbottom()->this_idx, RIGHT, masterid);
			apply_blend(chunk, cell.rightbottom()->this_idx, BOTTOM, masterid, true);
		}
	}
	
	if (!match(cell.lefttop())) {
		bool t = setid(cell.lefttop());
		if (!match(cell.top()) && (!match(cell.left()))) {
			apply_blend(chunk, cell.lefttop()->this_idx, LEFTTOP, masterid, t);
		} else if ((match(cell.top())) && (match(cell.left()))) {
			apply_blend(chunk, cell.lefttop()->this_idx, TOP, masterid);
			apply_blend(chunk, cell.lefttop()->this_idx, LEFT, masterid, true);
		}
	}
	
	if (!match(cell.righttop())) {
		bool t = setid(cell.righttop());
		if (!match(cell.right()) && (!match(cell.top()))) {
			apply_blend(chunk, cell.righttop()->this_idx, RIGHTTOP, masterid, t);
		} else if ((match(cell.right())) && (match(cell.top()))) {
			apply_blend(chunk, cell.righttop()->this_idx, TOP, masterid);
			apply_blend(chunk, cell.righttop()->this_idx, RIGHT, masterid, true);
		}
	}
	
}

void apply_blend(TeslaObject *chunk, std::size_t cell_idx, BlendDirection dir, std::size_t tile_id, bool overlay) {
	std::array<float, 6> blend_array {{ 0 }};
	switch (dir) {
		case LEFT:
			blend_array = {{ 0.0, 1.0, 1.0, 1.0, 0.0, 0.0 }}; break;
		case RIGHT:
			blend_array = {{ 1.0, 0.0, 0.0, 0.0, 1.0, 1.0 }}; break;
		case TOP:
			blend_array = {{ 0.0, 0.0, 1.0, 1.0, 1.0, 0.0 }}; break;
		case BOTTOM:
			blend_array = {{ 1.0, 1.0, 0.0, 0.0, 0.0, 1.0 }}; break;
		case LEFTTOP:
			blend_array = {{ 0.0, 0.0, 1.0, 1.0, 0.0, 0.0 }}; break;
		case LEFTBOTTOM:
			blend_array = {{ 0.0, 1.0, 0.0, 0.0, 0.0, 0.0 }}; break;
		case RIGHTTOP:
			blend_array = {{ 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 }}; break;
		case RIGHTBOTTOM:
			blend_array = {{ 1.0, 0.0, 0.0, 0.0, 0.0, 1.0 }}; break;
	}
	
	tesla_dataarray *data = chunk->get_data();
	tesla_drawcell& cell = data->cell(cell_idx);
	
	std::function<void (tesla_drawcell& cell, const std::array<float, 6>& weights)> func_setweights;
	
	std::function<void (tesla_drawcell& cell, const std::array<float, 6>& weights)> set_blendweights_x =
	[data, overlay] (tesla_drawcell& cell, const std::array<float, 6>& weights) {
		for (std::size_t i = 0; i < 6; i++)
			if (!overlay) data->vert(cell.vertices[i]).blendweights.x = weights[i];
			else data->vert(cell.vertices[i]).blendweights.x = std::min(cell.vert(i)->blendweights.x+weights[i], 1.0f);
	};
	
	std::function<void (tesla_drawcell& cell, const std::array<float, 6>& weights)> set_blendweights_y =
	[data, overlay] (tesla_drawcell& cell, const std::array<float, 6>& weights) {
		for (std::size_t i = 0; i < 6; i++)
			if (!overlay) data->vert(cell.vertices[i]).blendweights.y = weights[i];
			else data->vert(cell.vertices[i]).blendweights.y = std::min(cell.vert(i)->blendweights.y+weights[i], 1.0f);
	};
	
	if (cell.get_tileid() == tile_id) return;
	else if (cell.get_secondid() != cell.get_tileid()) {
		if (cell.get_secondid() != tile_id) {
			func_setweights = set_blendweights_y;
		} else {
			func_setweights = set_blendweights_x;
		}
	}
	
	func_setweights(cell, blend_array);
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

#define SERIAL(var) reinterpret_cast<char *>(&(var))
#define SERIALP(var) reinterpret_cast<char *>(var)
#define VOIDP(p) reinterpret_cast<void *>(p)
#define CHARP(p) reinterpret_cast<char *>(p)

std::string serialize_chunk(TeslaObject *chunk) {
	std::ostringstream ret;
	
	wonderland_chunk_header header;
	tesla_dataarray *data = chunk->get_data();
	header.num_cells = data->vec_cells().size();
	header.num_verts = data->vec_verts().size();
	header.num_idxes = data->vec_indexes().size();
	
	ret.write(SERIAL(header), sizeof(wonderland_chunk_header));
	ret.write(SERIALP(data->vec_indexes().data()), data->vec_indexes().size() * sizeof(GLIDX));
	ret.write(SERIALP(data->vec_verts().data()), data->vec_verts().size() * sizeof(tesla_vert));
	ret.write(SERIALP(data->vec_centers().data()), data->vec_centers().size() * sizeof(glm::vec3));

	for (std::size_t i = 0; i < header.num_cells; i++)
		ret.write(SERIAL(data->vec_cells()[i].vertices), CHARP(&(data->vec_cells()[i].t)) - CHARP(&(data->vec_cells()[i].vertices)));
	
	return ret.str();
}
		
tesla_dataarray *deserialize_chunk(const std::string& src, tesla_dataarray *dest) {
	std::istringstream in(src);
	
	wonderland_chunk_header header;
	in.read(SERIAL(header), sizeof(header));
	
	std::vector<tesla_vert>& vec_verts = dest->vec_verts();
	std::vector<GLIDX>& vec_indexes = dest->vec_indexes();
	std::vector<glm::vec3>& vec_centers = dest->vec_centers();
	std::vector<tesla_drawcell>& vec_cells = dest->vec_cells();
	vec_verts.clear();
	vec_indexes.clear();
	vec_centers.clear();
	vec_cells.clear();
	
	GLIDX index_t;
	for (std::size_t i = 0; i < header.num_idxes; i++) {
		in.read(SERIAL(index_t), sizeof(GLIDX));
		vec_indexes.push_back(index_t);
	}
	
	tesla_vert vert_t;
	for (std::size_t i = 0; i < header.num_verts; i++) {
		in.read(SERIAL(vert_t), sizeof(vert_t));
		vec_verts.push_back(vert_t);
	}
	
	glm::vec3 center_t;
	for (std::size_t i = 0; i < header.num_cells; i++) {
		in.read(SERIAL(center_t), sizeof(glm::vec3));
		vec_centers.push_back(center_t);
	}
	
	tesla_drawcell cell_t(dest);
	for (std::size_t i = 0; i < header.num_cells; i++) {
		in.read(SERIAL(cell_t.vertices), CHARP(&(cell_t.t)) - CHARP(&(cell_t.vertices)));
		vec_cells.push_back(cell_t);
	}
	
	return dest;
}

	}
	
}