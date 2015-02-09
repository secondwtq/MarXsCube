//
//  ModelLoader_obj.cpp
//  MarXsCube
//
//  Created by SeconDatke on 1/21/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "Common.h"

#include "ModelLoader_obj.h"

#include "cubegl.h"
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
			const unsigned int vert_idx = face[j];
			
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
			for (std::size_t j = 0; j < src.raw_faces.size(); j++) {
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
				this->raw_uvcoords.push_back({ stof(splits[1]), 30-stof(splits[2]), stof(splits[3]) });
			}
			
		}
	}
}