//
//  StdInterface.h
//  MarXsCube
//
//  Created by SeconDatke on 14-12-10.
//  Copyright (c) 2014年 MarXsCube Staff. All rights reserved.
//

#ifndef __MarXsCube__StdInterface__
#define __MarXsCube__StdInterface__

#include "Common.h"
#include "Config.h"

#include <string>
#include <vector>

namespace LuaInterface {
	
	using std_vector_int = std::vector<int, std::allocator<int>>;
	
	template <typename ValueT>
	void RegisterInterface_Std_Vector(LuaStatus &L, const std::string& name_type) {
		ValueT& (std::vector<ValueT>::*vec_at)(std::size_t) = &std::vector<ValueT>::at;
		
		luabridge::getGlobalNamespace(L).
		beginNamespace("Appins").
			beginNamespace("STL").
				beginNamespace("vector").
					beginClass<std::vector<ValueT> >(name_type.c_str()).
	//					addConstructor<void (*)(const vector&)>().
						addFunction("at", &vec_at).
	//					addFunction<void (*)(std_vector_int *, int)>("push_back", &std_vector_int::push_back).
						addFunction("empty", &std::vector<ValueT>::empty).
						addFunction("size", &std::vector<ValueT>::size).
						addFunction("length", &std::vector<ValueT>::size).
					endClass().
				endNamespace().
			endNamespace().
		endNamespace();
	}

	template <typename ValueT>
	void RegisterInterface_Std(LuaStatus &L, const std::string& name_type) {
		RegisterInterface_Std_Vector<ValueT>(L, name_type);
	}

}

#endif /* defined(__MarXsCube__StdInterface__) */
