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

using FaceSpec = std::array<std::size_t, 3>;
using VertexSpec = std::array<float, 3>;

class gl_vertarray;
class objfile;

void transfer_verts(gl_vertarray& dest, const objfile &src);

class gl_shader {
public:
	enum type {
		SHADER_VERTEX, SHADER_FRAG
	};
	
	void load_file(gl_shader::type type, const std::string &path);
	
	void load_str(gl_shader::type type, const char *src);
	
	void load_str(gl_shader::type type, const std::string& src);
	
	std::string log(gl_shader::type type);
	
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

class objfile {
public:
	std::string filepath { "" }	;
	
	std::vector<VertexSpec> raw_verts;
	std::vector<VertexSpec> raw_uvcoords;
	std::vector<FaceSpec> raw_faces;
	
	void parse();
	
	float **vert_array = nullptr;
};

class ObjLoader {
	
};

#endif /* defined(__MarXsCube__ModelLoader_obj__) */
