//
//  RenderElement.cpp
//  MarXsCube
//
//  Created by SeconDatke on 14-9-22.
//  Copyright (c) 2014年 MarXsCube Staff. All rights reserved.
//

#include "Common.h"
#include "SFML.h"
#include "Transform.h"
#include "InternalDraw.h"

#include "RenderElement.h"

using namespace sf;

void RenderElement_DirectionedStatic::_Render_Overload(CoordStruct &loc) {LOGFUNC;
	texture->CenterPivot(renderSprite);
	SetProjectionLocation_General(this, loc);
	renderSprite.setTexture(*texture);
	texture->setArea(renderSprite, getCurrentFrame());
	InternalDraw::DrawExt(*this, renderSprite);
}

void RenderElement_FramedStatic::_Render_Overload(CoordStruct &loc) {LOGFUNC;
	texture->CenterPivot(renderSprite);
	SetProjectionLocation_General(this, loc);
	renderSprite.setTexture(*texture);
	texture->setArea(renderSprite, currentFrame);
	InternalDraw::DrawExt(*this, renderSprite);
}

void RenderElement_FramedDynamic::_Render_Overload(CoordStruct &loc) {LOGFUNC;
	if (this->current_frame > this->frame_count) this-> current_frame = 1;
	
	texture->CenterPivot(renderSprite);
	SetProjectionLocation_General(this, loc);
	renderSprite.setTexture(*texture);
	texture->setArea(renderSprite, this->current_frame);
	
	this->current_frame++;
	
	InternalDraw::DrawExt(*this, renderSprite);
}

void RenderElement_InternalLine::_Render_Overload(CoordStruct &loc) {LOGFUNC;
	if (this->thickness == 0) {
		sf::Color sfcolor1(this->color1.x*255.0, this->color1.y*255.0, this->color1.z*255.0, this->color1.w*255.0);
		sf::Color sfcolor2(this->color2.x*255.0, this->color2.y*255.0, this->color2.z*255.0, this->color2.w*255.0);
		this->_verts[0] = sf::Vertex(obsTransform::GetViewPos(this->point1), sfcolor1);
		this->_verts[1] = sf::Vertex(obsTransform::GetViewPos(this->point2), sfcolor2);
		
		InternalDraw::DrawExt_InternalLine(*this, this->_verts, 2);
	}
}