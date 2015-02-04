//
//  GLFoundation.cpp
//  MarXsCube
//
//  Created by SeconDatke on 1/25/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "Common.h"
#include "SFML.h"

#include "ObjectManger.h"
#include "ModelLoader_obj.h"
#include "GLShader.h"
#include "GLFoundation.h"
#include "TilerRenderingBasic.h"
#include "ATVBCube.h"
#include "CubeTransform.h"

#include <SFML/OpenGL.hpp>
#include <iostream>

extern sf::RenderWindow *window_global;

gl_shader tiler_shader_main;

tiler_dataarray verts_data;
GLIDX texture_main = 0;
GLIDX texture_second = 0;
GLIDX texture_height = 0;
GLIDX texture_tileset = 0;

int vert_attrid = 0;
int vert_normid = 0;
int vert_textid = 0;
int vert_texcid = 0;
int vert_text_second_id = 0;
int vert_text_height_id = 0;
int vert_text_tileset_id = 0;
int vert_blendid = 0;
int vert_tileindexes = 0;
int vert_blend_secondaryid = 0;

GLuint vert_buf_new;
GLuint idx_buf;

objfile obj_test;

using namespace ATVBCube::Helper;

void GLFoundation::unbind_shader() {
	return glUseProgram(0); }

void load_obj() {
	obj_test.filepath = "drawcall.obj";
	obj_test.parse();
	
	transfer_verts_tiler(verts_data, obj_test);
}

const double GL_FACTOR_SCALE = (DIVS / 64) * TransformScaleFactor * 1.0;

void render_gl() {
	glm::vec3 look_at_vec = CubeTransform::look_at_vector();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(384, 384, 320, 0, 0, 0, 0, 0, 1);
	glTranslatef(look_at_vec.y, look_at_vec.x, 0);
	glScalef(GL_FACTOR_SCALE, GL_FACTOR_SCALE, GL_FACTOR_SCALE);
	
	glClear(GL_DEPTH_BUFFER_BIT);
	
	tiler_shader_main.use();
	
	glBindBuffer(GL_ARRAY_BUFFER, vert_buf_new);
	glVertexAttribPointer(vert_attrid, 3, GL_FLOAT, GL_FALSE, sizeof(tiler_dataarray::VertObjectType), (char *)0);
	glVertexAttribPointer(vert_normid, 3, GL_FLOAT, GL_FALSE, sizeof(tiler_dataarray::VertObjectType), (char *)(3*sizeof(GLfloat)));
	glVertexAttribPointer(vert_texcid, 3, GL_FLOAT, GL_FALSE, sizeof(tiler_dataarray::VertObjectType), (char *)(6*sizeof(GLfloat)));
	glVertexAttribPointer(vert_blendid, 3, GL_FLOAT, GL_FALSE, sizeof(tiler_dataarray::VertObjectType), (char *)(9*sizeof(GLfloat)));
	glVertexAttribPointer(vert_tileindexes, 3, GL_FLOAT, GL_FALSE, sizeof(tiler_dataarray::VertObjectType), (char *)(12*sizeof(GLfloat)));
	glEnableVertexAttribArray(vert_attrid);
	glEnableVertexAttribArray(vert_normid);
	glEnableVertexAttribArray(vert_texcid);
	glEnableVertexAttribArray(vert_blendid);
	glEnableVertexAttribArray(vert_tileindexes);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_main);
	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture_second);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texture_height);
	
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, texture_tileset);
	
	glUniform1i(vert_texcid, 0);
	glUniform1i(vert_text_second_id, 1);
	glUniform1i(vert_text_height_id, 2);
	glUniform1i(vert_text_tileset_id, 3);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idx_buf);
	glDrawElements(GL_TRIANGLES, (int)verts_data.count_idx(), GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	glDisableVertexAttribArray(vert_tileindexes);
	glDisableVertexAttribArray(vert_texcid);
	glDisableVertexAttribArray(vert_normid);
	glDisableVertexAttribArray(vert_attrid);
	glDisableVertexAttribArray(vert_blendid);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
}

void init_opengl() {
	
	window_global->setActive();
	
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glClearDepth(1.f);
	glDisable(GL_LIGHTING);
	
	GLsizei width = ATVBCube::setting<S::WindowSetting>().width,
			height = ATVBCube::setting<S::WindowSetting>().height;
	
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, height, 0, -16384.f, 16384.f);
	glScalef(1, -1, 1);
	
	tiler_shader_main.load_file(SHADERTYPE::VERTEX, "terrain_tiler.vert");
	tiler_shader_main.load_file(SHADERTYPE::FRAG, "terrain_tiler.frag");
	tiler_shader_main.create();
	
	vert_attrid = tiler_shader_main.get_attribute("position");
	vert_normid = tiler_shader_main.get_attribute("s_normal");
	
	vert_texcid = tiler_shader_main.get_attribute("s_texcoord");
	vert_blendid = tiler_shader_main.get_attribute("s_blendweights");
	vert_tileindexes = tiler_shader_main.get_attribute("s_texindexes");
	
	vert_textid = tiler_shader_main.get_uniform("s_texture_main");
	vert_text_second_id = tiler_shader_main.get_uniform("s_texture_second");
	vert_text_height_id = tiler_shader_main.get_uniform("s_texture_heightfield");
	vert_text_tileset_id = tiler_shader_main.get_uniform("s_texture_tileset");
	
	texture_main = TextureManger::GetInstance().TextureHashs["DOGE"]->texture.m_texture;
	texture_second = TextureManger::GetInstance().TextureHashs["JAGUAR"]->texture.m_texture;
	texture_height = TextureManger::GetInstance().TextureHashs["HEIGHTFIELD"]->texture.m_texture;
	texture_tileset = TextureManger::GetInstance().TextureHashs["TILESET"]->texture.m_texture;
	
	std::cout << tiler_shader_main.log(SHADERTYPE::VERTEX);
	std::cout << tiler_shader_main.log(SHADERTYPE::FRAG);
	
	load_obj();
	
	glGenBuffers(1, &vert_buf_new);
	glBindBuffer(GL_ARRAY_BUFFER, vert_buf_new);
	// a hack, roughly use the twice of original buffer size to create buffer
	//		cuz it seems the buffer size is fixed after initialized.
	glBufferData(GL_ARRAY_BUFFER, verts_data.count_vert() * sizeof(tiler_dataarray::VertObjectType)*1.5, verts_data.verts(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glGenBuffers(1, &idx_buf);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idx_buf);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, verts_data.count_idx() * sizeof(GLIDX), verts_data.indexes(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

#include "Generic.h"
#include "Transform.h"
#include "SilconThread.h"

#include <cmath>

void raise_verts() {
	tiler_dataarray::VertObjectType *verts = verts_data.verts();
	
	CoordStruct current_pos = obsTransform::GetWorldPos(Generic::Session()->MousePosData.pos);
	
	for (std::size_t i = 0; i < verts_data.count_vert(); i++) {
		glm::vec3 pos_xy = verts[i].position;
		glm::vec3 pos_cmp { current_pos.y, current_pos.x, pos_xy.z };
		
		auto d = glm::distance(pos_xy, pos_cmp);
		
		if (d < 96) {
//			float t = pow((96-d) / 96.0, 2);
//			verts[i].position.z += 32*t;
//			verts[i].blendweight[0] = std::min(verts[i].blendweight[0]+sqrt(t), 1.0f);
			verts[i].tile_indexes.x = 3.0;
		}
	}
	
	Acheron::Silcon.pause();
	glBindBuffer(GL_ARRAY_BUFFER, vert_buf_new);
	glBufferSubData(GL_ARRAY_BUFFER, 0, verts_data.count_vert() * sizeof(tiler_dataarray::VertObjectType), verts);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idx_buf);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, verts_data.count_idx() * sizeof(GLIDX), verts_data.indexes());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	Acheron::Silcon.invoke();
}

void tiler_array_test() {
	
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
}