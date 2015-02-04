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

using namespace FSMHelper;

namespace ATVBCube {
	
using namespace Helper;

namespace {
	CubeInit_WindowSetting _window_setting;
}
	
void ATVBSetting<BulletDebugSetting>::load() {
	enabled = value<bool>("bullet_debugdraw");
	scale = value<float>("bullet_debugdraw_scale");
}
	
void read_context_setting() {
	Generic::corelog()[L::Debug] << "Reading window setting..." << rn;
	
	sf::ContextSettings ret_context;
	ret_context.majorVersion = AtTheVeryBeginning::getatvb<unsigned int>("context_version_major");
	ret_context.minorVersion = AtTheVeryBeginning::getatvb<unsigned int>("context_version_minor");
	ret_context.antialiasingLevel = AtTheVeryBeginning::getatvb<unsigned int>("context_antialias");
	ret_context.stencilBits = 8;
	ret_context.depthBits = 24;
	
	_window_setting.context = ret_context;
	_window_setting.window_title = AtTheVeryBeginning::getatvb<std::string>("window_title") + " | MarXsCube Prototype";
	_window_setting.width = AtTheVeryBeginning::getatvb<unsigned int>("window_width");
	_window_setting.height = AtTheVeryBeginning::getatvb<unsigned int>("window_height");
	
	_window_setting.fps_limit = AtTheVeryBeginning::getatvb<unsigned int>("window_fps_limit");
	_window_setting.enable_vsync = AtTheVeryBeginning::getatvb<bool>("window_vsync");
}
	
CubeInit_WindowSetting& get_window_setting() { return _window_setting; }
	
}
