//
//  TeslaRendering.cpp
//  MarXsCube
//
//  Created by SeconDatke on 2/5/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "ATVBCube.h"
#include "GLFoundation.h"
#include "CubeTransform.h"
#include "TeslaObject.h"
#include "TeslaRendering.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

const float SCALE_FACTOR = TransformScaleFactor;

using namespace ATVBCube::Helper;

void TeslaRenderingManger::pre_render() {
	GLsizei width = ATVBCube::setting<S::WindowSetting>().width,
	height = ATVBCube::setting<S::WindowSetting>().height;
	
	using namespace glm;
	
	vec3 look_at_vec = CubeTransform::look_at_vector();
	
	mat4 model;
	mat4 view = lookAt<float, glm::precision::highp>({ 384.f, 384.f, 320.f }, { 0.f, 0.f, 0.f }, { 0.f, 0.f, 1.f });
	view = translate(view, vec3 { look_at_vec.y, look_at_vec.x, 0 } * SCALE_FACTOR);
	view = scale(view, vec3 { 1, 1, 1 } * SCALE_FACTOR);
	mat4 projection = ortho(0.f, (float)width, (float)height, 0.f, -16384.f, 16384.f);
	projection = scale(projection, vec3 { 1, -1, 1 });
	
	m_mat_mvp = projection * view * model;
	
	glDisable(GL_BLEND);
}

void TeslaRenderingManger::Render() {
	GLFoundation::clear_depth();
	
	for (auto chunk : this->chunks) {
		chunk->Render(); }

}