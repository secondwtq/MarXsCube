//
//  Serialization.h
//  MarXsCube
//
//  Created by SeconDatke on 2/22/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef __MarXsCube__Serialization__
#define __MarXsCube__Serialization__

#include "Common.h"
#include "Abstract.h"
#include "Object.h"
#include "Techno.h"

namespace boost {
namespace serialization {
	
	template <class Archive>
	void serialize(Archive& ar, Abs_Abstract& src, const unsigned int version) {
		ar & src.RTTIID;
		ar & src.EnablePhysics; }
	
	template <class Archive>
	void serialize(Archive& ar, CoordStruct& src, const unsigned int version) {
		ar & src.x, ar & src.y, ar & src.z; }
	
	template <class Archive>
	void serialize(Archive& ar, ObjectDirection& src, const unsigned int version) {
		ar & src.degree; }
	
	template <class Archive>
	void serialize(Archive& ar, Abs_Techno& src, const unsigned int version) {
		ar & src.RTTIID;
		ar & src.EnablePhysics;
		ar & src.Location;
		ar & src.Direction; }
	
}
}

#endif /* defined(__MarXsCube__Serialization__) */
