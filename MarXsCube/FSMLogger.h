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
		friend FSMLogger &get_logger(const std::string& name);
		
	public:
		
		FSMLogger() : FSMLogger("<UNKNOWN LOGGER>") { }
		
		FSMLogger(const char *name);
		FSMLogger(const std::string& name) : FSMLogger(name.c_str()) { }
		
		inline FSMLoggerProxy& operator[] (FSMLevel level) {
			return this->get_proxy(level); }
		
		inline FSMLoggerProxy& get_proxy() { return this->get_proxy(this->get_deflevel()); }
		FSMLoggerProxy& get_proxy(FSMLevel level);
		
		inline void set_logger(FSMBasicStream& logger) { this->m_logger = &logger; }
		inline FSMBasicStream &get_logger() { return *(this->m_logger); }
		inline void clear_logger() { this->m_logger = nullptr; }
		FSMLogger& set_deflogger();
		
		inline bool is_root() { return this->m_isroot; }
		inline const std::string& getname() { return this->m_name; }
		
		void log(const char *src);
		void log_noendl(const char *src);
		
		inline FSMLevel get_minlevel() { return this->m_minlevel; }
		inline FSMLogger &set_minlevel(FSMLevel level) { this->m_minlevel = level; return *this; }
		
		inline FSMLevel get_deflevel() { return this->m_defaultlevel; }
		inline FSMLogger &set_deflevel(FSMLevel level) { this->m_defaultlevel = level; return *this; }
		
	private:
		FSMLevel m_minlevel = FSMLevel::Debug;
		FSMLevel m_defaultlevel = FSMLevel::Debug;
		FSMBasicStream *m_logger = nullptr;
		bool m_isroot = false;
		std::vector<std::unique_ptr<FSMLoggerProxy> > m_proxies;
		std::string m_name = "<UNKNOWN LOGGER>";
	};
	
	class FSMLoggerProxy {
		
		friend FSMLoggerProxy create_handle();
		
	public:
		FSMLoggerProxy(FSMLogger& logger, FSMLevel level) : m_level(level), m_logger(&logger) { }
		
		inline FSMLoggerProxy& log(const char *src) {
			if (this->m_level >= this->m_logger->get_minlevel()) {
				if (this->inited) this->m_logger->log_noendl(src);
				else {
					this->inited = true;
					this->m_logger->log_noendl("[");
					this->m_logger->log_noendl(FSMLevelNames[static_cast<std::size_t>(this->m_level)]);
					this->m_logger->log_noendl("] ");
					this->m_logger->log_noendl(this->m_logger->getname().c_str());
					this->m_logger->log_noendl(" - ");
					this->m_logger->log_noendl(src);
				}
			}
			return *this;
		}
		
		inline FSMLoggerProxy& operator[] (FSMLevel level) {
			return this->m_logger->get_proxy(level); }
		
		inline FSMLoggerProxy& reset(bool inited = false) {
			this->inited = inited;
			return *this; }
		
	protected:
		FSMLevel m_level = FSMLevel::Debug;
		FSMLogger *m_logger = nullptr;
		bool inited = false;
		
	private:
		
		FSMLoggerProxy() { };
	};
	
	class FSMLoggerHandle : public FSMLoggerProxy {
		
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
	
	FSMLogger& get_logger(const std::string &name);
	void dispose_logger(const std::string &name);
	
	FSMLogger& logger();
	inline FSMLoggerProxy& logger(FSMLevel level) { return logger().get_proxy(level); }
	inline FSMLogger& logger(const std::string &name) { return get_logger(name); }
	
	inline FSMLoggerProxy create_handle() { return FSMLoggerProxy(); }

}

#include "FSMStream.h"

#endif /* defined(__MarXsCube__FSM__) */
