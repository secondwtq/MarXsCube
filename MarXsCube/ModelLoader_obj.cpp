//
//  ModelLoader_obj.cpp
//  MarXsCube
//
//  Created by SeconDatke on 1/21/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "Common.h"

#include "ModelLoader_obj.h"

#include <SFML/OpenGL.hpp>

#include <fstream>
#include <string>
#include <array>

#include <iostream>

#include <algorithm>
template <typename IterT>
IterT string_split(const std::string& src, const char delim, IterT out) {
	auto iter_first = src.begin();
	(*out).clear();
	std::for_each(iter_first, src.end(),
				  [&out, delim] (char o) { if (o == delim) { out++; (*out).clear(); }
											else (*out) += o; });
	return out;
}

const std::size_t LENGTH_SHADER_READBUFFER = 8192;

namespace {
	char _shader_readbuffer[LENGTH_SHADER_READBUFFER] { 0 };
}

// from http://blog.csdn.net/racehorse/article/details/6616256
std::string gl_shader::log(gl_shader::type type) {
	GLuint obj = (type == gl_shader::type::SHADER_FRAG) ? this->frag_id : this->vert_id;
	int length = 0, chwritten = 0;
	char *log_buffer;
	std::string ret = "";
	
	glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &length);
	
	if (length > 0) {
		log_buffer = (char *)malloc(length);
		glGetShaderInfoLog(obj, length, &chwritten, log_buffer);
		ret = log_buffer;
		free(log_buffer);
	}
	
	return ret;
}

void gl_shader::load_file(gl_shader::type type, const std::string &path) {
	FILE *fp = fopen(path.c_str(), "r");
	std::size_t len = fread(_shader_readbuffer, 1, LENGTH_SHADER_READBUFFER-1, fp);
	_shader_readbuffer[len] = '\0';
	this->load_str(type, _shader_readbuffer);
}

void gl_shader::load_str(gl_shader::type type, const char *src) {
	GLuint *target = (type == gl_shader::type::SHADER_FRAG) ? &this->frag_id : &this->vert_id;
	auto shader_tp = (type == gl_shader::type::SHADER_FRAG) ? GL_FRAGMENT_SHADER : GL_VERTEX_SHADER;
	
	*target = glCreateShader(shader_tp);
	const char * t[1] { src };
	glShaderSource(*target, 1, t, nullptr);
	glCompileShader(*target);
}

void gl_shader::load_str(gl_shader::type type, const std::string& src) {
	this->load_str(type, src.c_str()); }

void gl_shader::create() {
	this->obj_id = glCreateProgram();
	glAttachShader(this->obj_id, this->vert_id);
	glAttachShader(this->obj_id, this->frag_id);
	glLinkProgram(this->obj_id);
}

void gl_shader::use() {
	glUseProgram(this->obj_id);
}

int gl_shader::get_attribute(const char *name) {
	return glGetAttribLocation(this->obj_id, name);
}

int gl_shader::get_uniform(const char *name) {
	return glGetUniformLocation(this->obj_id, name);
}

void gl_vertarray::clear() {
	if (this->_data) {
		delete [] reinterpret_cast<float *>(this->_data);
		this->_data = nullptr;
		this->face(0);
	}
}

void transfer_verts(gl_vertarray& dest, const objfile &src) {
	dest.init_with(src.raw_faces.size());
	float (*data)[3][8] = (float (*) [3][8])(float ***)(dest.array());
	for (std::size_t i = 0; i < src.raw_faces.size(); i++) {
		const FaceSpec& face = src.raw_faces[i];
		
		auto p1 = src.raw_verts[face[0]], p2 = src.raw_verts[face[1]], p3 = src.raw_verts[face[2]];
		Float3D v1(p2[0]-p1[0], p2[1]-p1[1], p2[2]-p1[2]), v2(p3[0]-p1[0], p3[1]-p1[1], p3[2]-p1[2]);
		Float3D normal((v1.y*v2.z)-(v1.z*v2.y), -((v2.z*v1.x)-(v2.x*v1.z)), (v1.x*v2.y)-(v1.y*v2.x));
		
		for (std::size_t j = 0; j < 3; j++) {
			const std::size_t vert_idx = face[j];
			for (std::size_t k = 0; k < 3; k++)
				data[i][j][k] = src.raw_verts[vert_idx][k];
			
			data[i][j][3] = normal.x;
			data[i][j][4] = normal.y;
			data[i][j][5] = normal.z;
			
			for (std::size_t k = 0; k < 2; k++)
				data[i][j][k+6] = src.raw_uvcoords[vert_idx][k];
		}
	}
	
//	for (std::size_t i = 0; i < dest.len(); i++) {
//		if (i % 8 == 0) printf("\n");
//		printf("%.2f ", dest.array()[i]);
//	}
}

void objfile::parse() {
	this->raw_verts.push_back({0, 0, 0});
	this->raw_uvcoords.push_back({0, 0, 0}); // placeholders
	
	std::ifstream f(this->filepath);
	std::string t;
	
	while (getline(f, t)) {
		if (t.length() <= 1 || t[0] == '#') {
			
		} else {
			std::array<std::string, 5*2> splits;
			string_split(t, ' ', splits.begin());
			if (splits[0] == "v") {
				std::array<float, 3> vert;
				vert[0] = -std::stof(splits[1])*0.01;
				vert[1] = std::stof(splits[3])*0.01;
				vert[2] = std::stof(splits[2])*0.01;
				this->raw_verts.push_back(vert);
			} else if (splits[0] == "f") {
				std::array<std::size_t, 3> vert_idxs;
				std::array<std::string, 2> face_at;
				
				auto ivert = vert_idxs.begin();
				auto isplit = splits.begin()+1;
				
				while (ivert != vert_idxs.end()) {
					string_split(*isplit++, '/', face_at.begin());
					*ivert++ = std::stoul(face_at[0]);
				}
				this->raw_faces.push_back(vert_idxs);
			} else if (splits[0] == "vt") {
				std::array<float, 3> vert;
				vert[0] = std::stof(splits[1]);
				vert[1] = std::stof(splits[2]);
				vert[2] = std::stof(splits[3]);
				this->raw_uvcoords.push_back(vert);
			}
		}
	}
}