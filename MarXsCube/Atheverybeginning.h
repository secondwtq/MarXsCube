//
//  Atheverybeginning.h
//  MarXsCube
//
//  Created by SeconDatke on 1/22/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef __MarXsCube__Atheverybeginning__
#define __MarXsCube__Atheverybeginning__

#include "Common.h"
#include "Config.h"

namespace AtTheVeryBeginning {

void init_atvb(LuaStatus &L);
void dispose_atvb();

template <typename T>
T getatvb(const std::string& key);
	
template<typename T>
void atvb_setdefault(const std::string& key, const T& value);
	
void load_config(const std::string& filename);
	
class AtTheVeryBeginning {
	
	friend luabridge::LuaRef atvb_get(const std::string& key);

	template<typename T>
	friend void atvb_setdefault(const std::string& key, const T& value);
	
	friend void load_config(const std::string& filename);
	
public:
	
	AtTheVeryBeginning();
	
	void init(LuaStatus &L);
	void register_interface();
	
private:
	LuaStatus *_l = nullptr;
	luabridge::LuaRef *_nil_ref = nullptr;
};

}

#endif /* defined(__MarXsCube__Atheverybeginning__) */
