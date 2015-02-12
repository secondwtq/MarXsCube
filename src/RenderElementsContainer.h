//
//  RenderElementsContainer.h
//  MarXsCube
//
//  Created by SeconDatke on 14-9-22.
//  Copyright (c) 2014年 MarXsCube Staff. All rights reserved.
//

#ifndef __MarXsCube__RenderElementsContainer__
#define __MarXsCube__RenderElementsContainer__

#include "Common.h"
#include "SFML.h"
#include "Abstract.h"
#include "RenderElement.h"

#include <unordered_map>

class RenderElementsContainer {
public:
	std::unordered_multimap<int, RenderElement *> elements;
	Abs_Abstract *parent;
	void pre_update();
	void Update();
	RenderElementsContainer(Abs_Abstract *_parent) : parent(_parent) {LOGFUNC; }
	void insert(int Zidx, RenderElement *element) {LOGFUNC;
		elements.insert({Zidx, element});
	}
	
	~RenderElementsContainer() {
		for (auto i : elements)
			delete i.second;
	}
	
	int direction = 0;
};

#endif /* defined(__MarXsCube__RenderElementsContainer__) */
