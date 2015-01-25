//
//  CubeTransform.cpp
//  MarXsCube
//
//  Created by SeconDatke on 1/25/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "CubeTransform.h"

#include <glm/glm.hpp>

namespace CubeTransform {
	
	using namespace glm;

	mat4 ct_view_mat;
	mat4 ct_view_mat_temp;
	
	vec3 ct_camera_pos { 384, 384, 320 };
	vec3 ct_look_at { 0, 0, 0 };
	vec3 ct_vec_up { 0, 0, 1 };
	vec3 ct_vec_zero { 0, 0, 0 };
	
	void generate_view_matrix() {
		vec3 za = normalize(ct_camera_pos - ct_look_at);
		vec3 xa = normalize(cross(ct_vec_up, za));
		vec3 ya = cross(za, xa);
		
//		ct_view_mat_temp = mat4(xa, ya, za, ct_vec_zero);
	}
	
}