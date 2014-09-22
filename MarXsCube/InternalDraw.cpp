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

void InternalDraw::Draw(Drawable &obj) {
	LOGFUNC;
	static auto shader_ptr = &TestManger::GetInstance().extShader;
	TestManger::GetInstance().window->draw(obj, shader_ptr);
}

void InternalDraw::DrawExt(RenderElement &objArg, Drawable &obj) {
	LOGFUNC;
	static auto shader_ptr = &TestManger::GetInstance().extShader;
	shader_ptr->setParameter("colorMultiply", objArg.colorMultiply.x, objArg.colorMultiply.y, objArg.colorMultiply.z, objArg.colorMultiply.w);
	TestManger::GetInstance().window->draw(obj, shader_ptr);
}