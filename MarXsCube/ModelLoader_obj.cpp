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
#include <glm/glm.hpp>

#include <fstream>
#include <string>
#include <array>

#include <iostream>

#include <unordered_map>

#include <algorithm>
template <typename IterT>
IterT string_split(const std::string& src, const char delim, IterT out) {
	auto iter_first = src.begin();
	(*out).clear();
	std::for_each(iter_first, src.end(),
				  [&out, delim] (char o) { if (o == delim) { if ((*out).length()) { out++; (*out).clear(); } }
											else (*out) += o; });
	return out;
}

const std::size_t LENGTH_SHADER_READBUFFER = 8192;

namespace {
	char _shader_readbuffer[LENGTH_SHADER_READBUFFER] { 0 };
}

// from http://blog.csdn.net/racehorse/article/details/6616256
std::string gl_shader::log(SHADERTYPE type) {
	GLuint obj = (type == SHADERTYPE::FRAG) ? this->frag_id : this->vert_id;
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

void gl_shader::load_file(SHADERTYPE type, const std::string &path) {
	FILE *fp = fopen(path.c_str(), "r");
	std::size_t len = fread(_shader_readbuffer, 1, LENGTH_SHADER_READBUFFER-1, fp);
	_shader_readbuffer[len] = '\0';
	this->load_str(type, _shader_readbuffer);
}

void gl_shader::load_str(SHADERTYPE type, const char *src) {
	GLuint *target = (type == SHADERTYPE::FRAG) ? &this->frag_id : &this->vert_id;
	auto shader_tp = (type == SHADERTYPE::FRAG) ? GL_FRAGMENT_SHADER : GL_VERTEX_SHADER;
	
	*target = glCreateShader(shader_tp);
	const char * t[1] { src };
	glShaderSource(*target, 1, t, nullptr);
	glCompileShader(*target);
}

void gl_shader::load_str(SHADERTYPE type, const std::string& src) {
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
	
	// so dirty!
	float (*data)[3][8] = (float (*) [3][8])(float ***)(dest.array());
	
	for (std::size_t i = 0; i < src.raw_faces.size(); i++) {
		const glm::i32vec3& face = src.raw_faces[i];
		
		auto p1 = src.raw_verts[face[0]], p2 = src.raw_verts[face[1]], p3 = src.raw_verts[face[2]];
		glm::vec3 v1 = p2 - p1, v2 = p3 - p1;
		glm::vec3 normal { (v1.y*v2.z)-(v1.z*v2.y), -((v2.z*v1.x)-(v2.x*v1.z)), (v1.x*v2.y)-(v1.y*v2.x) };
		
		for (GLIDX j = 0; j < 3; j++) {
			const int vert_idx = face[j];
			
			// vertex position
			for (GLIDX k = 0; k < 3; k++)
				data[i][j][k] = src.raw_verts[vert_idx][k];
			
			// normals
			if (src.raw_normals.size() > vert_idx)
				for (GLIDX k = 0; k < 3; k++)
					data[i][j][k+3] = src.raw_normals[vert_idx][k];
			else
				for (GLIDX k = 0; k < 3; k++)
					// if there is no normal, use calculated normals
					data[i][j][k+3] = normal[k];
			
			// uv coords
			for (GLIDX k = 0; k < 2; k++)
				data[i][j][k+6] = src.raw_uvcoords[src.raw_face_uvcoords[i][j]][k];
		}
	}
	
//	for (std::size_t i = 0; i < dest.len(); i++) {
//		if (i % 8 == 0) printf("\n");
//		printf("%.2f ", dest.array()[i]);
//	}
}

void transfer_verts_idx(gl_vertarray_indexed& dest, const objfile &src) {
	dest.init_with(src.raw_verts.size(), src.raw_faces.size()*3);
	
	std::unordered_map<std::size_t, std::size_t> texcoord_cache;
	
	gl_vert_object *dest_verts = dest.verts();
	
	for (std::size_t i = 0; i < src.raw_verts.size(); i++) {
		const glm::i32vec3& face = src.raw_faces[i];
		
		auto p1 = src.raw_verts[face[0]], p2 = src.raw_verts[face[1]], p3 = src.raw_verts[face[2]];
		glm::vec3 v1 = p2 - p1, v2 = p3 - p1;
		glm::vec3 normal { (v1.y*v2.z)-(v1.z*v2.y), -((v2.z*v1.x)-(v2.x*v1.z)), (v1.x*v2.y)-(v1.y*v2.x) };
		
		dest_verts[i].position = src.raw_verts[i];
		if (src.raw_normals.size() > i)
			dest_verts[i].normal = src.raw_normals[i];
		else
			dest_verts[i].normal = normal;
		
		if (texcoord_cache.find(i) != texcoord_cache.end()) {
			dest_verts[i].texcoord = src.raw_uvcoords[texcoord_cache.at(i)];
		} else {
			for (int j = 0; j < src.raw_faces.size(); j++) {
				for (int k = 0; k < 3; k++) {
					auto vert_idx = src.raw_faces[j][k];
					texcoord_cache[vert_idx] = src.raw_face_uvcoords[j][k];
					if (vert_idx == i) {
						dest_verts[i].texcoord = src.raw_uvcoords[src.raw_face_uvcoords[j][k]];
						break;
					}
				}
			}
		}
		
	}
	
	GLIDX *dest_idxs = dest.indexes();
	
	for (std::size_t i = 0; i < src.raw_faces.size(); i++) {
		glm::i32vec3 current_face = src.raw_faces[i];
		for (int j = 0; j < 3; j++)
			dest_idxs[i*3+j] = current_face[j];
	}
}

void objfile::parse() {
	
	// placeholders
	this->raw_verts.push_back({0, 0, 0});
	this->raw_normals.push_back({0, 0, 0});
	this->raw_uvcoords.push_back({0, 0, 0});
	
	using namespace std;
	using namespace glm;
	
	ifstream f(this->filepath);
	string t;
	
	while (getline(f, t)) {
		if (t.length() <= 1 || t[0] == '#') {
			
		} else {
			
			array<string, 5*2> splits;
			string_split(t, ' ', splits.begin());
			
			if (splits[0] == "v")		// read vertexs
				this->raw_verts.push_back(vec3(stof(splits[3]), stof(splits[1]), stof(splits[2])));
			else if (splits[0] == "vn")		// read vertex normals
				this->raw_normals.push_back({ stof(splits[3]), stof(splits[1]), stof(splits[2]) });
			
			else if (splits[0] == "f") {		// read meshes
				i32vec3 vert_idxs, vert_uvcoords;
				array<string, 3> face_at;
				
				auto isplit = splits.begin()+1;
				for (GLIDX i = 0; i < 3; i++) {
					string_split(*isplit++, '/', face_at.begin());
					vert_idxs[i] = stoi(face_at[0]);
					vert_uvcoords[i] = stoi(face_at[1]);
				}
				this->raw_faces.push_back(vert_idxs);
				this->raw_face_uvcoords.push_back(vert_uvcoords);
			}
			
			else if (splits[0] == "vt") {		// read texture coords
				if (!splits[3].length()) splits[3] = "0";
				this->raw_uvcoords.push_back({ stof(splits[1]), stof(splits[2]), stof(splits[3]) });
			}
			
		}
	}
}