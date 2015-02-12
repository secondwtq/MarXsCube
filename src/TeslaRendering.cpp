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
	
	glm::vec3 look_at_vec = CubeTransform::look_at_vector();
	
	glm::mat4 model;
	glm::mat4 view = glm::lookAt<float, glm::precision::highp>({ 384.f, 384.f, 320.f }, { 0.f, 0.f, 0.f }, { 0.f, 0.f, 1.f });
	view = glm::translate(view, glm::vec3 { look_at_vec.y, look_at_vec.x, 0 } * SCALE_FACTOR);
	view = glm::scale(view, glm::vec3 { 1, 1, 1 } * SCALE_FACTOR);
	glm::mat4 projection = glm::ortho(0.f, (float)width, (float)height, 0.f, -16384.f, 16384.f);
	projection = glm::scale(projection, glm::vec3 { 1, -1, 1 });
	
	m_mat_mvp = projection * view * model;
}

void TeslaRenderingManger::Render() {
	glm::vec3 look_at_vec = CubeTransform::look_at_vector();
	GLFoundation::clear_depth();
	GLFoundation::view(look_at_vec.x, look_at_vec.y);
	
	for (auto chunk : this->chunks) {
		chunk->Render(); }
	
	GLFoundation::view(look_at_vec.x, look_at_vec.y);
}