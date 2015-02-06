//
//  GritAStar.cpp
//  MarXsCube
//
//  Created by SeconDatke on 2/6/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "GritAStar.h"

float GritAStar::GA_HEURIST_WEIGHT = 1.f;

void GritAStar::path(GritNode& start, GritNode& end, std::vector<GritNode *>& all_nodes) {
	
	int n = 0;
	
	GritAStar::pri_queue<GritNode> open, close;
	
	open.push(&start);
	start.cost = 0;
	start.cost_est = estimate(start, end, GA_HEURIST_WEIGHT);
	
	GritNode *current = nullptr;
	
	while (open.size()) {
		current = open.front();
		
		if (current == &end)
			break;
		
		std::vector<std::size_t> *links = &current->links;
		
		for (std::size_t i = 0; i < links->size(); i++) {
			GritNode *end_node = all_nodes[(*links)[i]];
			
			float cost_inc = cost(*current, *end_node);
			float cost_end = current->cost + cost_inc;
			
			if (close.contains(end_node)) {
				if (end_node->cost <= cost_end)
					continue;
				
				close.remove(end_node);
			} else if (open.contains(end_node)) {
				if (end_node->cost <= cost_end)
					continue;
			}
			
			float node_end_heurist = estimate(*end_node, end, GA_HEURIST_WEIGHT);
			end_node->cost = cost_end;
			end_node->parent = current;
			end_node->cost_est = cost_end + node_end_heurist;
			
			if (!open.contains(end_node))
				open.push(end_node);
		}
		
		close.push(current);
		open.remove(current);
		
		n++;
		if (n > 300) return; // magic again, - -
	}
	
	// WIP
}