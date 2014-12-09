//
//  InternalDraw.h
//  MarXsCube
//
//  Created by SeconDatke on 14-9-22.
//  Copyright (c) 2014年 MarXsCube Staff. All rights reserved.
//

#ifndef __MarXsCube__InternalDraw__
#define __MarXsCube__InternalDraw__

#include "Common.h"
#include "SFML.h"

class RenderElement;

class InternalDraw {
public:
	static void Draw(sf::Drawable &obj);
	
	static void DrawExt(RenderElement &objArg, sf::Drawable &obj);
	
	static void DrawExt_InternalLine(RenderElement &objArg, sf::Vertex *verts, unsigned int count_verts);
};

#include "RenderElement.h"

#endif /* defined(__MarXsCube__InternalDraw__) */
