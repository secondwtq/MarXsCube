//
//  Obsoleted.cpp
//  MarXsCube
//
//  Created by SeconDatke on 1/24/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

/*
void thread_rendering() {
	std::unique_lock<std::mutex> lk(mut_render);
	
	update_render.wait(lk, [] { return should_render; });
	
	window_global->setActive();
	while (true) {
		if (!game_running) return lk.unlock();
		
		counter_render++;
		float fps = counter_render / clock_render.getElapsedTime().asSeconds();
		printf("rendering... %lu %f\n", counter_render, fps);
		window_global->clear(sf::Color::Black);
		
		window_global->pushGLStates();
		for (size_t i = 0; i < RenderLayerType::Count; i++)
			Generic::RenderLayerManger()->Layers[i].Update();
		window_global->popGLStates();
		
		render_gl();
		
		//		should_render = false;
		window_global->display();
		//		lk.unlock();
	}
}
*/
