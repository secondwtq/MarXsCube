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
GLIDX texture_main = 0;

int vert_attrid = 0;
int vert_normid = 0;
int vert_textid = 0;
int vert_texcid = 0;

GLuint vert_buf;

void load_obj() {
	objfile obj_test;
	obj_test.filepath = "drawcall.obj";
	obj_test.parse();
	
	transfer_verts(verts_def, obj_test);
}

extern glm::vec3 gl_campos;
extern glm::vec3 gl_lookat;

void render_gl() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(384, 384, 320, 0, 0, 0, 0, 0, 1);
	glTranslatef(gl_lookat.x, gl_lookat.y, gl_lookat.z);
	
	glClear(GL_DEPTH_BUFFER_BIT);
	
	tiler_shader_main.use();
	glBindBuffer(GL_ARRAY_BUFFER, vert_buf);
	//	glVertexPointer(3, GL_FLOAT, 5*sizeof(GLfloat), (char *)0);
	glVertexAttribPointer(vert_attrid, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (char *)0);
	glVertexAttribPointer(vert_normid, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (char *)(3*sizeof(GLfloat)));
	glVertexAttribPointer(vert_texcid, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (char *)(6*sizeof(GLfloat)));
	glEnableVertexAttribArray(vert_attrid);
	glEnableVertexAttribArray(vert_normid);
	glEnableVertexAttribArray(vert_texcid);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_main);
	glUniform1i(vert_textid, 0);
	glDrawArrays(GL_TRIANGLES, 0, (int)verts_def.len());
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
//	glOrtho(-1.f, 1.f, -1.f/ratio, 1.f/ratio, .1f, 500.f);
	
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
	
	//	glDisableClientState(GL_NORMAL_ARRAY);
	//	glDisableClientState(GL_COLOR_ARRAY);
}