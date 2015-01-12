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

int RenderElement_DirectionedStatic::getCurrentFrame() { return getDirFrameNum(this->parent->direction, frameCount); }

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
	this->shape.m_thickness = this->thickness;
	this->shape.setPoints(obsTransform::GetViewPos(this->point1), obsTransform::GetViewPos(this->point2));
	this->shape.setFillColor(sf::Color(this->color.x*255.0, this->color.y*255.0, this->color.z*255.0, this->color.w*255.0));
	InternalDraw::Draw(this->shape);
}
