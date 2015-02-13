//
//  InternalDraw.cpp
//  MarXsCube
//
//  Created by SeconDatke on 14-9-22.
//  Copyright (c) 2014年 MarXsCube Staff. All rights reserved.
//

#include "Common.h"
#include "SFML.h"
#include "ObjectManger.h"
#include "InternalDraw.h"
#include "SilconSprite.h"

void InternalDraw::Draw(Drawable &obj) {
	TestManger::GetInstance().window->draw(obj); }

void InternalDraw::DrawExt(RenderElement &objArg, Drawable &obj) {
	TestManger::GetInstance().window->draw(obj); }

void InternalDraw::DrawExt_InternalLine(RenderElement &objArg, sf::Vertex *verts, unsigned int count_verts) {
	TestManger::GetInstance().window->draw(verts, count_verts, sf::Lines); }