//
//  Startup.cpp
//  MarXsCube
//
//  Created by SeconDatke on 2/2/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "FSM.h"
#include "Atheverybeginning.h"
#include "ATVBCube.h"
#include "Generic.h"

#include "Startup.h"

#include <stdio.h>
#include "unistd.h"

extern sf::RenderWindow *window_global;

using namespace FSMHelper;

namespace {
	const std::size_t BUFSIZE_WORKINGDIR_PATH = 1024;
	char workingdir_buffer[BUFSIZE_WORKINGDIR_PATH] { 0 };
}

void cube_init_workingdir();

void cube_init_first() {
	printf("MarXsCube by seCOnDatkE, 2014-2015.\n\n");
	printf("Initing FSM ...\n");
	
	Generic::Init_Logger();
	
	cube_init_workingdir();
}

void cube_init_workingdir() {
	getcwd(workingdir_buffer, BUFSIZE_WORKINGDIR_PATH);
	Generic::corelog()[L::Debug] << "Working directory: " << workingdir_buffer << rn;
}

void cube_init_atvb() {
	Generic::corelog()[L::Debug] << "Initing AtTheVeryBeginning ..." << rn;
	AtTheVeryBeginning::init_atvb(*Generic::lua_state());
	AtTheVeryBeginning::load_config("atheverybeginning.lua");
}

void cube_init_window() {
	ATVBCube::read_context_setting();
	
	const ATVBCube::CubeInit_WindowSetting *window_setting = &ATVBCube::get_window_setting();
	
	Generic::corelog()[L::Debug] << "Creating window..." << rn;
	window_global = new sf::RenderWindow(sf::VideoMode(window_setting->width, window_setting->height), window_setting->window_title, sf::Style::Titlebar || sf::Style::Close, window_setting->context);
	
	sf::ContextSettings settings_got = window_global->getSettings();
	Generic::corelog()[L::Debug] << "Running with OpenGL " << settings_got.majorVersion << "." << settings_got.minorVersion << rn;
	
	window_global->setFramerateLimit(ATVBCube::get_window_setting().fps_limit);
	window_global->setVerticalSyncEnabled(ATVBCube::get_window_setting().enable_vsync);
}