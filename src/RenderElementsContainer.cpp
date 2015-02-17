//
//  RenderElementsContainer.cpp
//  MarXsCube
//
//  Created by SeconDatke on 14-9-22.
//  Copyright (c) 2014年 MarXsCube Staff. All rights reserved.
//

#include "Common.h"
#include "RenderElement.h"
#include "RenderElementsContainer.h"

void RenderElementsContainer::Update() {
	for (auto e : elements) e->Update(parent->GetCoord()); }