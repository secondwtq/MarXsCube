//
//  Gmap_Interface.cpp
//  MarXsCube
//
//  Created by SeconDatke on 14-12-10.
//  Copyright (c) 2014年 MarXsCube Staff. All rights reserved.
//

#include "Common.h"

#include "graph_def.h"
#include "bellman_ford.h"

#include "Gmap_Interface.h"

using namespace luabridge;

void RegisterInterface_User_Gmap(LuaStatus &L) {
	getGlobalNamespace(L).
	beginNamespace("Appins").
		beginNamespace("Gmap").
			beginClass<gGraph>("Graph").
				addConstructor<void (*)(std::size_t)>().
				addFunction("count_vert", &gGraph::count_vert).
				addFunction("count_edge", &gGraph::count_edge).
				addFunction("connect", &gGraph::connect).
				addFunction("connected", &gGraph::connected).
				addFunction("marked", &gGraph::marked).
				addFunction("mark", &gGraph::mark).
				addFunction("clear_mark", &gGraph::clear_mark).
			endClass().
			beginClass<bellman_food_shortest>("bellman_ford_shortest").
				addConstructor<void (*)(gGraph *, std::size_t)>().
				addFunction("go", &bellman_food_shortest::go).
			endClass().
		endNamespace().
	endNamespace();
}