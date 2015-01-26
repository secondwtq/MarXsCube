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

#include "GLFoundation.h"

#include <SFML/OpenGL.hpp>
#include <iostream>

extern sf::RenderWindow *window_global;

gl_vertarray verts_def;
gl_shader tiler_shader_main;

gl_vertarray_indexed verts_idx_def;
GLIDX texture_main = 0;

int vert_attrid = 0;
int vert_normid = 0;
int vert_textid = 0;
int vert_texcid = 0;

GLuint vert_buf;

GLuint vert_buf_new;
GLuint idx_buf;

void load_obj() {
	objfile obj_test;
	obj_test.filepath = "drawcall.obj";
	obj_test.parse();
	
	transfer_verts(verts_def, obj_test);
	transfer_verts_idx(verts_idx_def, obj_test);
}

extern glm::vec3 gl_campos;
extern glm::vec3 gl_lookat;

const double GL_FACTOR_SCALE = (DIVS / 10) * TransformScaleFactor * 1.1;

void render_gl() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(384, 384, 320, 0, 0, 0, 0, 0, 1);
	glTranslatef(gl_lookat.x, gl_lookat.y, gl_lookat.z);
	glScalef(GL_FACTOR_SCALE, GL_FACTOR_SCALE, GL_FACTOR_SCALE);
	
	glClear(GL_DEPTH_BUFFER_BIT);
	
	tiler_shader_main.use();
	
	glBindBuffer(GL_ARRAY_BUFFER, vert_buf_new);
	glVertexAttribPointer(vert_attrid, 3, GL_FLOAT, GL_FALSE, 9*sizeof(float), (char *)0);
	glVertexAttribPointer(vert_normid, 3, GL_FLOAT, GL_FALSE, 9*sizeof(float), (char *)(3*sizeof(GLfloat)));
	glVertexAttribPointer(vert_texcid, 3, GL_FLOAT, GL_FALSE, 9*sizeof(float), (char *)(6*sizeof(GLfloat)));
	glEnableVertexAttribArray(vert_attrid);
	glEnableVertexAttribArray(vert_normid);
	glEnableVertexAttribArray(vert_texcid);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_main);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glUniform1i(vert_textid, 0);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idx_buf);
	glDrawElements(GL_TRIANGLES, (int)verts_idx_def.count_idx(), GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	glDisableVertexAttribArray(vert_texcid);
	glDisableVertexAttribArray(vert_normid);
	glDisableVertexAttribArray(vert_attrid);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void init_opengl() {
	
	window_global->setActive();
	
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glClearDepth(1.f);
	glDisable(GL_LIGHTING);
	
	GLdouble width = static_cast<double>(window_global->getSize().x),
	height = static_cast<double>(window_global->getSize().y);
	
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
	vert_textid = tiler_shader_main.get_attribute("s_texture_main");
	vert_texcid = tiler_shader_main.get_attribute("s_texcoord");
	
	texture_main = TextureManger::GetInstance().TextureHashs["DOGE"]->texture.m_texture;
	
	std::cout << tiler_shader_main.log(SHADERTYPE::VERTEX);
	std::cout << tiler_shader_main.log(SHADERTYPE::FRAG);
	
	load_obj();
	
	glGenBuffers(1, &vert_buf);
	glBindBuffer(GL_ARRAY_BUFFER, vert_buf);
	glBufferData(GL_ARRAY_BUFFER, verts_def.len() * sizeof(float), verts_def.array(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glGenBuffers(1, &vert_buf_new);
	glBindBuffer(GL_ARRAY_BUFFER, vert_buf_new);
	glBufferData(GL_ARRAY_BUFFER, verts_idx_def.count_vert() * sizeof(gl_vert_object), verts_idx_def.verts(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glGenBuffers(1, &idx_buf);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idx_buf);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, verts_idx_def.count_idx() * sizeof(GLIDX), verts_idx_def.indexes(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	//	glDisableClientState(GL_NORMAL_ARRAY);
	//	glDisableClientState(GL_COLOR_ARRAY);
}