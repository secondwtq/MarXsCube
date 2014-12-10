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

#include <vector>

#include "Gmap_Interface.h"

using namespace luabridge;

using namespace Gmap;

namespace Gmap {
	
	// Function to extract path (with nodes) from Gmap::bellman_ford_shortest::edge_to
	//	and return a LuaRef table
	//	mapped to Appins::Gmap::bellman_ford_shortest::extract_path()
	//		do it with two passes, one get the reversed path, the second reverse it while coping data to the table
	// Attention: the return value does not include the start node.
	LuaRef bf_extract_path(bellman_food_shortest const *src, std::size_t dest, lua_State *L) {
		std::size_t current_node = dest;
		const Gmap::gGraph_listnode *current_edge = src->edge_to[current_node];
		std::vector<std::size_t> nodes;
		while (current_edge) {
			nodes.push_back(current_node);
			current_node = current_edge->other(current_node);
			current_edge = src->edge_to[current_node];
		}
		
		LuaRef ret = luabridge::newTable(L);
		std::size_t idx = 1;
		for (auto i = nodes.rbegin(); i != nodes.rend(); i++)
			ret[idx++] = *i;
		return ret;
	}
}

namespace LuaInterface {

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
				addStaticFunction("extract_path", &Gmap::bf_extract_path).
			endClass().
		endNamespace().
	endNamespace();
}

}