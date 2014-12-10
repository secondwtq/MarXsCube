//
//  bellman_ford.h
//  MarXsCube
//
//  Created by SeconDatke on 14-12-10.
//  Copyright (c) 2014年 MarXsCube Staff. All rights reserved.
//

#ifndef MarXsCube_bellman_ford_h
#define MarXsCube_bellman_ford_h

#include "graph_def.h"

namespace Gmap {

class bellman_food_shortest {
public:
	std::vector<const gGraph_listnode *> edge_to;
	
	// we need a copy ctor for security,
	//	but be awared: it is not to be copied, naturally.
	//		if it is copied somehow (LuaBridge bug, for most time), the dtor would be called twice
	//			then is time for OS.
	
	bellman_food_shortest(gGraph *src, std::size_t rootidx)
		: src_graph(src), root(rootidx), inside_queue(new bool[src_graph->count_vert()]) {
		for (int i = 0; i < src->count_vert(); i++) {
			this->dist_to.push_back(0xFFFFFF);
			this->edge_to.push_back(nullptr);
		}
		this->dist_to[rootidx] = 0;
	}
	
	~bellman_food_shortest() { delete this->inside_queue; }
	
	void go() {
		memset(inside_queue, 0, src_graph->count_vert() * sizeof(bool));
		this->qu.push(this->root);
		this->inside_queue[this->root] = true;
		
		while (!qu.empty()) {
			std::size_t v = this->qu.front();
			this->qu.pop();
			this->inside_queue[v] = false;
			relax_pass(v);
		}
	}
	
private:
	
	gGraph *src_graph;
	std::vector<std::size_t> dist_to;
	std::size_t root = 0;
	
	bool *inside_queue;
	std::queue<std::size_t> qu;
	
	void relax_pass(std::size_t v) {
		for (auto l : this->src_graph->adj(v)) {
			std::size_t target = l->other(v);
			
			if (this->dist_to[target] > this->dist_to[v] + l->weight) {
				this->dist_to[target] = this->dist_to[v] + l->weight;
				
				this->edge_to[target] = l;
				
				if (!this->inside_queue[target]) {
					this->qu.push(target);
					this->inside_queue[target] = true;
				}
			}
		}
	}
	
};
	
}

#endif
