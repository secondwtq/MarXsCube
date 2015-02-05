//
//  TilerBullet.cpp
//  MarXsCube
//
//  Created by SeconDatke on 1/27/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "PhysicsCommon.h"
#include "Physics.h"

#include "ModelLoader_obj.h"
#include "Generic.h"
#include "ATVBCube.h"
#include "TilerBullet.h"

using namespace ATVBCube::Helper;

#include "SFML.h"

sf::Color sfImageGetPixelFloat(const sf::Image& image, float x, float y) {
	auto size = image.getSize();
	return image.getPixel(x*size.x, y*size.y);
}

void transfer_bullet_shape(btTriangleMesh& dest, const objfile &src, const sf::Texture *heightfield) {
	float max_height = ATVBCube::setting<S::TilerGeneralSetting>().maxheight_phy;
	
	sf::Image heightimage = heightfield->copyToImage();
	
	for (std::size_t i = 0; i < src.raw_faces.size(); i++) {
		glm::i32vec3 face = src.raw_faces[i];
		
		int height1 = 0, height2 = 0, height3 = 0;
		
		glm::i32vec3 texcoord = src.raw_face_uvcoords[i];
		glm::vec3 gt1 = src.raw_uvcoords[texcoord[0]], gt2 = src.raw_uvcoords[texcoord[1]], gt3 = src.raw_uvcoords[texcoord[2]];
		glm::vec3 gv1 = src.raw_verts[face[0]], gv2 = src.raw_verts[face[1]], gv3 = src.raw_verts[face[2]];
		
		gt1 /= 30.0, gt2 /= 30.0, gt3 /= 30.0;
		float heightf1 = sfImageGetPixelFloat(heightimage, gt1.x, gt1.y).r/255.0,
				heightf2 = sfImageGetPixelFloat(heightimage, gt2.x, gt2.y).r/255.0,
				heightf3 = sfImageGetPixelFloat(heightimage, gt3.x, gt3.y).r/255.0;
		
		height1 = heightf1 * max_height, height2 = heightf2 * max_height, height3 = heightf3 * max_height;
		
		gv1 *= PHY_SCALE, gv2 *= PHY_SCALE, gv3 *= PHY_SCALE;
		height1 *= PHY_SCALE, height2 *= PHY_SCALE, height3 *= PHY_SCALE;
		
		btVector3 bv1 { gv1.y, gv1.x, gv1.z+height1 }, bv2 { gv2.y, gv2.x, gv2.z+height2 }, bv3 { gv3.y, gv3.x, gv3.z+height3 };
		dest.addTriangle(bv1, bv2, bv3);
	}
}
