//
//  SilconThread.cpp
//  MarXsCube
//
//  Created by SeconDatke on 1/24/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "Common.h"

#include "Generic.h"
#include "Acheron.h"

#include "SilconThread.h"

#include "SFML.h"

extern sf::RenderWindow *window_global;

#include "GLFoundation.h"

std::size_t counter_render = 0;
sf::Clock clock_render;

void silcon_acheron_function() {
	counter_render++;
	float fps = counter_render / clock_render.getElapsedTime().asSeconds();
//	printf("rendering... %lu %f\n", counter_render, fps);
	window_global->clear(sf::Color::Black);

	render_gl();
	
	window_global->pushGLStates();
	for (size_t i = 0; i < RenderLayerType::Count; i++)
		Generic::RenderLayerManger()->Layers[i].Update();
	window_global->popGLStates();
	
	window_global->display();
}

Acheron::AcheronBase Acheron::Silcon(silcon_acheron_function);
