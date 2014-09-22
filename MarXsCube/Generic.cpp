//
//  Generic.cpp
//  MarXsCube
//
//  Created by SeconDatke on 14-9-22.
//  Copyright (c) 2014年 MarXsCube Staff. All rights reserved.
//

#include "Common.h"

#include "Session.h"
#include "ObjectManger.h"

#include "Generic.h"

class Session *Generic::session = nullptr;
class RenderLayerManger *Generic::render_layer_manger = nullptr;