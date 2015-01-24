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

#include <stdio.h>

#include <string>
#include <vector>
#include <iostream>
#include <memory>

enum class FSMLevel {
	Highest,
	Fatal,
	Error,
	Warning,
	Debug,
	Message,
	Trace,
	Lowest,
};

extern const char * FSMLevelNames[];

namespace FSM {
	
	class FSMLogger;
	class FSMLoggerProxy;
	
	void init();
	void dispose();
	bool inited();
	
	class FSMBasicStream {
	public:
		
		virtual void close() = 0;
		
		virtual void write(const char *src) = 0;
		virtual void writeln(const char *src) = 0;
		
		virtual ~FSMBasicStream() { };
		
	};
	
	class FSMStdStream : public FSMBasicStream {
	
		friend void init();
		
	public:
		
		virtual void close();
		
		virtual void write(const char *src);
		virtual void writeln(const char *src);
		
		virtual bool is_root() { return this->m_isroot; }
		
	private:
		
		bool m_isroot = false;
		
	};
	
	class FSMFileStream : public FSMBasicStream {
	public:
		
		virtual void close();
		
		virtual void write(const char *src);
		virtual void writeln(const char *src);
		
	private:
		
		std::shared_ptr<std::ofstream> m_fs = nullptr;
	};
	
	// http://stackoverflow.com/questions/3165563/flexible-logger-class-using-standard-streams-in-c
	// really nice!
	class FSMCompositeStream : public FSMBasicStream {
	public:
		
		virtual void close();
		
		virtual FSMCompositeStream& add_logger(FSMBasicStream &src);
		
		virtual void write(const char *src);
		virtual void writeln(const char *src);
		
	private:
		std::vector<FSMBasicStream *> m_loggers;
	};
	
	class FSMLogger {
		
		friend void init();
		
	public:
		
		FSMLogger();
		
		FSMLoggerProxy& operator[] (FSMLevel level);
		
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
		
		FSMLoggerProxy& log(const char *src) {
			if (this->inited) {
				this->m_logger->log_noendl(src);
			} else {
				this->inited = true;
				this->m_logger->log_noendl(FSMLevelNames[static_cast<std::size_t>(this->m_level)]);
				this->m_logger->log_noendl(": ");
				this->m_logger->log_noendl(src);
			}
			return *this;
		}
		
	private:
		FSMLevel m_level = FSMLevel::Debug;
		FSMLogger *m_logger = nullptr;
		bool inited = false;
	};
	
	class LoggerEndline { };
	using rn = LoggerEndline;
	
	template <typename T>
	std::string& convert_to_string(const T& src) {
		static std::string unknown { "FSM::convert_to_string() - [UNKNOWN CONVERTION]" };
		return unknown; }
	
	template<typename T>
	FSMLoggerProxy& operator << (FSMLoggerProxy& proxy, const T& src) {
		return proxy.log(convert_to_string(src).c_str());
	}
	
	template<>
	FSMLoggerProxy& operator << <LoggerEndline>(FSMLoggerProxy& proxy, const LoggerEndline& src) {
		return proxy.log(CUBE_ENDLINE); }
	
	template<>
	FSMLoggerProxy& operator << <const char *>(FSMLoggerProxy& proxy,  const char * const& src) {
		return proxy.log(src); }
	
	FSMLogger& logger();
	
	FSMLogger& get_logger(const std::string &name);
	
	void dispose_logger(const std::string &name);
}

#endif /* defined(__MarXsCube__FSM__) */
