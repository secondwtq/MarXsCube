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
	std::vector<RenderElement *> elements;
	Abs_Abstract *parent;
	void Update();
	RenderElementsContainer(Abs_Abstract *_parent) : parent(_parent) { }
	void insert(RenderElement *element) {
		elements.push_back(element); }
	
	~RenderElementsContainer() {
		for (auto i : elements)
			delete i; }
	
	int direction = 0;
};

#endif /* defined(__MarXsCube__RenderElementsContainer__) */
