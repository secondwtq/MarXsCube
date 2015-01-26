//
//  ModelLoader_obj.h
//  MarXsCube
//
//  Created by SeconDatke on 1/21/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef __MarXsCube__ModelLoader_obj__
#define __MarXsCube__ModelLoader_obj__

#include "Common.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <array>

#include <glm/glm.hpp>

using FaceSpec = std::array<std::size_t, 3>;

class objfile;
class gl_vertarray;
class gl_vertarray_indexed;

void transfer_verts(gl_vertarray& dest, const objfile &src);

enum SHADERTYPE {
	VERTEX, FRAG, PROGRAM };

class gl_shader {
public:
	
	void load_file(SHADERTYPE type, const std::string &path);
	
	void load_str(SHADERTYPE type, const char *src);
	
	void load_str(SHADERTYPE type, const std::string& src);
	
	std::string log(SHADERTYPE type);
	
	void create();
	
	int get_attribute(const char *name);
	int get_uniform(const char *name);
	
	void use();
	
	unsigned int obj_id = 0;
	unsigned int vert_id = 0, frag_id = 0;
};

class gl_vertarray {
public:
	
	void clear();
	float *array() { return this->_data; }
	std::size_t count_face() { return this->_len; }
	std::size_t len() { return this->_len * 3 * 8; }
	void face(std::size_t len) { this->_len = len; }
	void init_with(std::size_t facecount) {
		this->clear();
		this->face(facecount);
		this->_data = new float [this->len()];
	}
	
	virtual ~gl_vertarray() { this->clear(); }
	
private:
	float *_data = nullptr;
	std::size_t _len = 0;
	
};

struct gl_vert_object {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 texcoord;
	glm::vec3 blendweight { 0 };
};

void transfer_verts_idx(gl_vertarray_indexed& dest, const objfile &src);

class gl_vertarray_indexed {
public:
	
	void clear() {
		this->m_len_vert = 0;
		delete [] this->m_vert_data;
		this->m_len_idx = 0;
		delete [] this->m_idx_data;
	}
	
	void init_with(std::size_t count_vert, std::size_t count_idx) {
		this->m_len_vert = count_vert;
		this->m_len_idx = count_idx;
		this->m_vert_data = new gl_vert_object[m_len_vert];
		this->m_idx_data = new GLIDX[m_len_idx];
	}
	
	gl_vert_object *verts() { return this->m_vert_data; }
	GLIDX *indexes() { return this->m_idx_data; }
	
	std::size_t count_vert() { return this->m_len_vert; }
	std::size_t count_idx() { return this->m_len_idx; }

private:
	gl_vert_object *m_vert_data = nullptr;
	GLIDX *m_idx_data = nullptr;
	
	std::size_t m_len_vert = 0;
	std::size_t m_len_idx = 0;
};

class objfile {
public:
	std::string filepath { "" }	;
	
	std::vector<glm::vec3> raw_verts, raw_uvcoords, raw_normals;
	std::vector<glm::i32vec3> raw_faces, raw_face_uvcoords;
	
	void parse();
	
	float **vert_array = nullptr;
};

class ObjLoader {
	
};

#endif /* defined(__MarXsCube__ModelLoader_obj__) */
