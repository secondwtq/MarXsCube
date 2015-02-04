//
//  ATVBCube.h
//  MarXsCube
//
//  Created by SeconDatke on 2/2/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef __MarXsCube__ATVBCube__
#define __MarXsCube__ATVBCube__

#include "SFML.h"
#include "Atheverybeginning.h"

#include <string>

namespace ATVBCube {
	
	enum ATVBSettingType {
		WindowSetting,
		BulletDebugSetting,
	};
	
	namespace Helper {
		using S = ATVBSettingType;
	}
	
	template <ATVBSettingType SettingT>
	class ATVBSettingP {
		public:
		
			template <typename T>
			T value(const std::string& key) {
				return AtTheVeryBeginning::getatvb<T>(key); }
		
			virtual void load() { }
		
			virtual ~ATVBSettingP() { }
		
			static ATVBSettingP<SettingT> *instance() { return ATVBSettingP<SettingT>::m_instance; }
		
		protected:
			static ATVBSettingP<SettingT> *m_instance;
		
	};
	
	template <ATVBSettingType SettingT>
	class ATVBSetting : public ATVBSettingP<SettingT> {
	public:
		
		void load() { }
		
	};
	
	template <>
	class ATVBSetting<BulletDebugSetting> : public ATVBSettingP<BulletDebugSetting> {
	public:
		
		bool enabled = false;
		float scale = 1.0f;
		
		void load();
		
	};
	
	template <ATVBSettingType SettingT>
	inline ATVBSetting<SettingT>& setting() {
		return (*static_cast<ATVBSetting<SettingT> *>(ATVBSetting<SettingT>::instance())); }
	
	template <ATVBSettingType SettingT>
	inline void load() {
		return setting<SettingT>().load(); }
	
	template <ATVBSettingType SettingT>
	ATVBSettingP<SettingT> *ATVBSettingP<SettingT>::m_instance = new ATVBSetting<SettingT>();
	
	struct CubeInit_WindowSetting {
	public:
		sf::ContextSettings context;
		
		std::string window_title;
		
		unsigned int width, height;
		
		unsigned int fps_limit;
		bool enable_vsync;
	};
	
	void read_context_setting();
	
	CubeInit_WindowSetting& get_window_setting();
	
}

#endif /* defined(__MarXsCube__ATVBCube__) */
