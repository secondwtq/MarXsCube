//
//  CubeTransform.cpp
//  MarXsCube
//
//  Created by SeconDatke on 1/25/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "Common.h"
#include "SFML.h"

#include "CubeTransform.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace CubeTransform {
	
	using namespace glm;

	mat4 ct_view_mat;
	
	vec3 ct_camera_pos { 384, 384, 320 };
	vec3 ct_look_at { 0, 0, 0 };
	vec3 ct_vec_up { 0, 0, 1 };
	
	void update_view_matrix() {
		ct_view_mat = glm::lookAt(ct_camera_pos, ct_look_at, ct_vec_up);
	}
	
	vec3 look_at_vector() { return ct_look_at; }
	
	void generate_view_matrix(int ox, int oy) {
		ct_look_at = { -ox, -oy, 0 };
		ct_camera_pos = { 384-ox, 384-oy, 320 };

		update_view_matrix();
	}
	
	sf::Vector2f view_pos(const CoordStruct &coord) {
		vec4 ret { (-coord.x), (-coord.y), (-coord.z), 1 };
		ret = ct_view_mat * ret * float(TransformScaleFactor);
		return sf::Vector2f(ret[0], ret[1]); }
	
	glm::vec2 view_pos_nt(const CoordStruct &coord) {
		vec4 ret { (-coord.x), (-coord.y), (-coord.z), 1 };
		ret = ct_view_mat * ret * float(TransformScaleFactor);
		return glm::vec2(ret.x, ret.y); }
	
}