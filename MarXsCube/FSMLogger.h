//
//  FSM.h
//  MarXsCube
//
//  Created by SeconDatke on 1/24/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef __MarXsCube__FSM__
#define __MarXsCube__FSM__

#include "Common.h"

#include "FSMCommon.h"

#include <stdio.h>

#include <string>
#include <vector>
#include <iostream>
#include <memory>

namespace FSM {
	
	class FSMLogger;
	class FSMLoggerProxy;
	class FSMBasicStream;
	
	class FSMLogger {
		
		friend void init();
		
	public:
		
		FSMLogger();
		
		inline FSMLoggerProxy& operator[] (FSMLevel level) {
			return this->get_proxy(level); }
		
		FSMLoggerProxy& get_proxy(FSMLevel level);
		
		void set_logger(FSMBasicStream& logger) { this->m_logger = &logger; }
		FSMBasicStream &get_logger() { return *(this->m_logger); }
		void clear_logger() { this->m_logger = nullptr; }
		
		bool is_root() { return this->m_isroot; }
		
		void log(const char *src);
		void log_noendl(const char *src);
		
		FSMLevel get_minlevel() { return this->m_minlevel; }
		FSMLogger &set_minlevel(FSMLevel level) { this->m_minlevel = level; return *this; }
		
		FSMLevel get_deflevel() { return this->m_defaultlevel; }
		FSMLogger &set_deflevel(FSMLevel level) { this->m_defaultlevel = level; return *this; }
		
	private:
		FSMLevel m_minlevel = FSMLevel::Debug;
		FSMLevel m_defaultlevel = FSMLevel::Debug;
		FSMBasicStream *m_logger = nullptr;
		bool m_isroot = false;
		std::vector<std::unique_ptr<FSMLoggerProxy> > m_proxies;
	};
	
	class FSMLoggerProxy {
	public:
		FSMLoggerProxy(FSMLogger& logger, FSMLevel level) : m_level(level), m_logger(&logger) { }
		
		inline FSMLoggerProxy& log(const char *src) {
			if (this->m_level >= this->m_logger->get_minlevel()) {
				if (this->inited) {
					this->m_logger->log_noendl(src);
				} else {
					this->inited = true;
					this->m_logger->log_noendl(FSMLevelNames[static_cast<std::size_t>(this->m_level)]);
					this->m_logger->log_noendl(": ");
					this->m_logger->log_noendl(src);
				}
			}
			return *this;
		}
		
	private:
		FSMLevel m_level = FSMLevel::Debug;
		FSMLogger *m_logger = nullptr;
		bool inited = false;
	};
	
	template <typename T>
	inline std::string& convert_to_string(const T& src) {
		static std::string unknown { "FSM::convert_to_string() - [UNKNOWN CONVERTION]" };
		return unknown; }

	template<typename T>
	inline FSMLoggerProxy& operator << (FSMLoggerProxy& proxy, const T& src) {
		return proxy.log(convert_to_string(src).c_str()); }
	
	inline FSMLoggerProxy& operator << (FSMLoggerProxy& proxy,  const char *src) {
		return proxy.log(src); }
	
	void init();
	void dispose();
	bool inited();
	
	FSMLogger& logger();
	inline FSMLoggerProxy& logger(FSMLevel level) { return logger().get_proxy(level); }
	
	FSMLogger& get_logger(const std::string &name);
	
	void dispose_logger(const std::string &name);
}

#include "FSMStream.h"

#endif /* defined(__MarXsCube__FSM__) */
