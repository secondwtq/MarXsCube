//
//  ATVBCube.cpp
//  MarXsCube
//
//  Created by SeconDatke on 2/2/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "Atheverybeginning.h"
#include "Generic.h"
#include "FSM.h"

#include "ATVBCube.h"

namespace ATVBCube {

using namespace FSMHelper;
using namespace Helper;
	
void ATVBSetting<WindowSetting>::load() {
	sf::ContextSettings ret_context;
	ret_context.majorVersion = value<unsigned int>("context_version_major");
	ret_context.minorVersion = value<unsigned int>("context_version_minor");
	ret_context.antialiasingLevel = value<unsigned int>("context_antialias");
	ret_context.stencilBits = 8;
	ret_context.depthBits = 24;
	context = ret_context;
	
	window_title = value<std::string>("window_title") + " | MarXsCube Prototype";
	width = value<unsigned int>("window_width");
	height = value<unsigned int>("window_height");
	
	fps_limit = value<unsigned int>("window_fps_limit");
	enable_vsync = value<bool>("window_vsync");
}
	
void ATVBSetting<BulletDebugSetting>::load() {
	enabled = value<bool>("bullet_debugdraw");
	scale = value<float>("bullet_debugdraw_scale");
}
	
void ATVBSetting<BulletGeneralSetting>::load() {
	gravity = value<Float3D>("bullet_gravity");
}
	
void ATVBSetting<TeslaGeneralSetting>::load() {
	maxheight_phy = value<float>("tiler_max_height_bullet");
}
	
}
