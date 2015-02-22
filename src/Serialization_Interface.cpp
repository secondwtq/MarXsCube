//
//  Serialization_Interface.cpp
//  MarXsCube
//
//  Created by SeconDatke on 2/22/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "Config.h"
#include "Techno.h"
#include "Serialization.h"
#include "Serialization_Interface.h"

#include <string>
#include <sstream>

#include <boost/archive/text_oarchive.hpp>

std::string serialize_techno(Abs_Techno *src) {
	std::ostringstream ss;
	
	boost::archive::text_oarchive oa(ss);
	oa << *src;
	
	return ss.str();
}

void LuaInterface::RegisterInterface_Serialization(LuaStatus &L) {
	luabridge::getGlobalNamespace(L).
		beginNamespace("Serialization").
			addFunction("serialize_techno", &serialize_techno).
		endNamespace();
}