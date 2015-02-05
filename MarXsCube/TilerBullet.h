//
//  TilerBullet.h
//  MarXsCube
//
//  Created by SeconDatke on 1/27/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef __MarXsCube__TilerBullet__
#define __MarXsCube__TilerBullet__

#include "SFML.h"
#include "PhysicsCommon.h"
#include "ModelLoader_obj.h"

void transfer_bullet_shape(btTriangleMesh& dest, const objfile &src, const sf::Texture *heightfield);

#endif /* defined(__MarXsCube__TilerBullet__) */
