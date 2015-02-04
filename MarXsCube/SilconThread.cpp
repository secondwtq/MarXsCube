//
//  SilconThread.cpp
//  MarXsCube
//
//  Created by SeconDatke on 1/24/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "Common.h"
#include "SFML.h"

#include "Generic.h"
#include "Acheron.h"
#include "GLFoundation.h"
#include "BulletDebugDraw.h"
#include "ATVBCube.h"

#include "SilconThread.h"

using namespace ATVBCube::Helper;

extern sf::RenderWindow *window_global;

void silcon_acheron_function() {
	window_global->clear(sf::Color::Black);

	render_gl();
	
	window_global->pushGLStates();
	for (size_t i = 0; i < RenderLayerType::Count; i++)
		Generic::RenderLayerManger()->Layers[i].Update();
	window_global->popGLStates();
	
	if (ATVBCube::setting<S::BulletDebugSetting>().enabled)
		BulletDebugDrawer::render();
	
	window_global->display();
}

Acheron::AcheronBase Acheron::Silcon(silcon_acheron_function);
