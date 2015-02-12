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
#include "TeslaRendering.h"
#include "BulletDebugDraw.h"
#include "ATVBCube.h"
#include "Timer.h"
#include "Silcon.h"
#include "SilconSprite.h"
#include "SilconThread.h"

using namespace ATVBCube::Helper;

extern sf::RenderWindow *window_global;

fps_counter fps_silcon;

void silcon_acheron_function() {
	fps_silcon.update();
	if (fps_silcon.updated) printf("Rendering FPS: %lf\n", fps_silcon.fps);
	window_global->clear(sf::Color::Black);

	Generic::TeslaManger()->pre_render();
	Generic::TeslaManger()->Render();
	
	for (size_t i = 0; i < RenderLayerType::Count; i++)
		Generic::RenderLayerManger()->Layers[i].Update();
	Silcon::Manger->render_dispatch();
	
//	if (ATVBCube::setting<S::BulletDebugSetting>().enabled)
//		BulletDebugDrawer::render();
	
	window_global->display();
}

Acheron::AcheronBase Acheron::Silcon(silcon_acheron_function);
