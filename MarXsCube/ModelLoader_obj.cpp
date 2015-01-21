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

void gl_vertarray::clear() {
	if (this->_data) {
		delete [] reinterpret_cast<float *>(this->_data);
		this->_data = nullptr;
		this->face(0);
	}
}

void transfer_verts(gl_vertarray& dest, const objfile &src) {
	dest.init_with(src.raw_faces.size());
	float (*data)[3][3] = (float (*) [3][3])(float ***)(dest.array());
	for (std::size_t i = 0; i < src.raw_faces.size(); i++) {
		const FaceSpec& face = src.raw_faces[i];
		for (std::size_t j = 0; j < 3; j++) {
			std::size_t vert_idx = face[j];
			for (std::size_t k = 0; k < 3; k++)
				data[i][j][k] = src.raw_verts[vert_idx][k];
		}
	}
}

void objfile::parse() {
	this->raw_verts.push_back({0, 0, 0}); // placeholder
	
	std::ifstream f(this->filepath);
	std::string t;
	
	while (getline(f, t)) {
		if (t.length() <= 1 || t[0] == '#') {
			
		} else {
			std::array<std::string, 5*2> splits;
			string_split(t, ' ', splits.begin());
			if (splits[0] == "v") {
				std::array<float, 3> vert;
				vert[0] = std::stof(splits[1])*0.01;
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
			}
		}
	}
}