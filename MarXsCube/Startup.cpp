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
#include "Version.h"
#include "Startup.h"

#include <stdio.h>
#include "unistd.h"

extern sf::RenderWindow *window_global;

using namespace FSMHelper;
using namespace ATVBCube::Helper;

namespace {
	const std::size_t BUFSIZE_WORKINGDIR_PATH = 1024;
	char workingdir_buffer[BUFSIZE_WORKINGDIR_PATH] { 0 };
}

void cube_init_workingdir();

void cube_init_first() {
	printf(CUBE_FULL_NAME " by secondwtq, 2014-2015. \n\n");
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
	ATVBCube::load<S::WindowSetting>();
	
	Generic::corelog()[L::Debug] << "Creating window..." << rn;
	window_global = new sf::RenderWindow(sf::VideoMode(ATVBCube::setting<S::WindowSetting>().width, ATVBCube::setting<S::WindowSetting>().height),
						ATVBCube::setting<S::WindowSetting>().window_title, sf::Style::Titlebar || sf::Style::Close, ATVBCube::setting<S::WindowSetting>().context);
	
	sf::ContextSettings settings_got = window_global->getSettings();
	Generic::corelog()[L::Debug] << "Running with OpenGL " << settings_got.majorVersion << "." << settings_got.minorVersion << rn;
	
//	window_global->setFramerateLimit(ATVBCube::setting<S::WindowSetting>().fps_limit);
//	window_global->setVerticalSyncEnabled(ATVBCube::setting<S::WindowSetting>().enable_vsync);
	
}