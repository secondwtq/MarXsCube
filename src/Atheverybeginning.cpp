//
//  Atheverybeginning.cpp
//  MarXsCube
//
//  Created by SeconDatke on 1/22/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "Common.h"
#include "HafniumCommon.h"
#include "Atheverybeginning.h"

#include <unordered_map>

using namespace std;
using namespace luabridge;

namespace AtTheVeryBeginning {

namespace {
	std::unordered_map<std::string, luabridge::LuaRef> atvb_map;
	luabridge::LuaRef *atvb_proxy = nullptr;
	AtTheVeryBeginning *atvb_sig;
	
	// http://stackoverflow.com/questions/410980/include-a-text-file-in-a-c-program-as-a-char
	std::string atvb_init_script =
		#include "atheverybeginning.lunc"
	;
}
	
void init_atvb(LuaStatus &L) {
	atvb_sig = new AtTheVeryBeginning();
	atvb_sig->init(L);
	
	atvb_proxy = new luabridge::LuaRef(L.State);
	*atvb_proxy = luabridge::newTable(L.State);
	
	atvb_sig->register_interface();
	
	luaL_dostring(L, atvb_init_script.c_str());
}

template <typename T>
T cast_to(const LuaRef& src) {
	return src.cast<T>(); }
	
template <>
Float3D cast_to(const LuaRef& src) {
	return { src[1].cast<float>(), src[2].cast<float>(), src[3].cast<float>() }; }

template<typename T>
T getatvb(const std::string& key) {
	if (atvb_map.find(key) != atvb_map.end())
		return cast_to<T>(atvb_map.at(key));
	else {
		cout << "AtTheVeryBeginning::getatvb - no value found for key " << key << endl;
		return T();
	}
}

template<typename T>
void atvb_setdefault(const std::string& key, const T& value) {
	if (atvb_map.find(key) == atvb_map.end())
		atvb_map.insert({ key, LuaRef(atvb_sig->_l->State, value) });
	else atvb_map.at(key) = LuaRef(atvb_sig->_l->State, value);
}

void load_config(const std::string& filename) {
	hafnium_dofile(atvb_sig->_l->State, filename.c_str()); }

luabridge::LuaRef _atvb_get(LuaRef table, LuaRef key) {
	//		LuaRef p = (*atvb_proxy)[key];
	return atvb_map.at(key.cast<std::string>()); }

void _atvb_set(LuaRef table, LuaRef key, LuaRef value) {
	const std::string &tk = key.cast<std::string>();
	if (atvb_map.find(tk) == atvb_map.end())
		atvb_map.insert({ tk, value });
	else atvb_map.at(tk) = value;
}

AtTheVeryBeginning::AtTheVeryBeginning() { }

void AtTheVeryBeginning::init(LuaStatus &L) {
	this->_l = &L;
	this->_nil_ref = luabridge::LuaRef(this->_l->State);
}

void AtTheVeryBeginning::register_interface() {
	luabridge::getGlobalNamespace(*this->_l).
		beginNamespace("__AtTheVeryBeginning").
			addFunction("set", &_atvb_set).
			addFunction("get", &_atvb_get).
		endNamespace();
}

template bool getatvb<bool>(const std::string& key);
template std::string getatvb<std::string>(const std::string& key);
template unsigned int getatvb<unsigned int>(const std::string& key);
template float getatvb<float>(const std::string& key);
template Float3D getatvb<Float3D>(const std::string& key);
template std::size_t getatvb<std::size_t>(const std::string& key);

}