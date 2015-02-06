//
//  StdInterface.cpp
//  MarXsCube
//
//  Created by SeconDatke on 2/6/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "Common.h"
#include "StdInterface.h"

#include <vector>

namespace LuaInterface {

template <typename ValueT>
void RegisterInterface_Std_Vector(LuaStatus &L, const char *name_type) {
	ValueT& (std::vector<ValueT>::*vec_at)(std::size_t) = &std::vector<ValueT>::at;
	void (std::vector<ValueT>::*vec_push_back)(const ValueT&) = &std::vector<ValueT>::push_back;
	
	luabridge::getGlobalNamespace(L).
		beginNamespace("Utility").
			beginNamespace("std").
				beginNamespace("vector").
					beginClass<std::vector<ValueT> >(name_type).
						addFunction("at", vec_at).
						addFunction("empty", &std::vector<ValueT>::empty).
						addFunction("size", &std::vector<ValueT>::size).
						addFunction("length", &std::vector<ValueT>::size).
						addFunction("push_back", vec_push_back).
					endClass().
				endNamespace().
			endNamespace().
		endNamespace();
}

}

template <typename ValueT>
void LuaInterface::RegisterInterface_Std(LuaStatus &L, const char *name_type) {
	RegisterInterface_Std_Vector<ValueT>(L, name_type);
}

template void LuaInterface::RegisterInterface_Std<int>(LuaStatus &L, const char *name_type);
template void LuaInterface::RegisterInterface_Std<CubePoint>(LuaStatus &L, const char *name_type);