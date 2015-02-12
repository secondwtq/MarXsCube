//
//  RenderElement.cpp
//  MarXsCube
//
//  Created by SeconDatke on 14-9-22.
//  Copyright (c) 2014年 MarXsCube Staff. All rights reserved.
//

#include "Common.h"
#include "SFML.h"

#include "CubeTransform.h"
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

template <class T>
inline void SetProjectionLocation_NT(T *element, CoordStruct& loc) {
	if (!element->UseShadowProjection)
		element->sprite.position = CubeTransform::view_pos_nt(loc+element->offset);
	else {
		auto _loc = loc + element->offset;
		_loc = CoordStruct(element->ProjectionVector.z*_loc.x+element->ProjectionVector.x*_loc.z, element->ProjectionVector.z*_loc.y+element->ProjectionVector.y*_loc.z, 0);
		element->sprite.position = CubeTransform::view_pos_nt(_loc);
	}
}

inline float silcon_dis_camera(const CoordStruct& loc) {
	static auto d = Generic::Session()->CameraLocation;
	int f = d.x*10-loc.x, g = d.y*10-loc.y, h = d.z*10-loc.z;
	return (f*f+g*g+h*h);
}

void RenderElement_FramedStatic::_update_overload(CoordStruct &loc) {
	SetProjectionLocation_NT(this, loc);
	this->sprite.color_multiply = { this->colorMultiply.x, this->colorMultiply.y, this->colorMultiply.z, this->colorMultiply.w };
	this->sprite.set_Zvalue(-1.f*silcon_dis_camera(loc)/16384.f);
}

void RenderElement_FramedStatic::_Render_Overload(CoordStruct &loc) {
	this->sprite.render();
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
	this->shape.setPoints(CubeTransform::view_pos(this->point1), CubeTransform::view_pos(this->point2));
	this->shape.setFillColor(sf::Color(this->color.x*255.0, this->color.y*255.0, this->color.z*255.0, this->color.w*255.0));
	InternalDraw::Draw(this->shape);
}
