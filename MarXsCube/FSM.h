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
	Fatal,
	Error,
	Warning,
	Debug,
	Message,
	Trace,
};

namespace FSM {
	
	void init();
	void dispose();
	
	class FSMBasicStream {
	public:
		
		virtual void close() = 0;
		
		virtual void write(const char *src) = 0;
		virtual void writeln(const char *src) = 0;
		
		virtual ~FSMBasicStream();
		
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
		
		void set_logger(FSMBasicStream& logger) { this->m_logger = &logger; }
		FSMBasicStream &get_logger() { return *this->m_logger; }
		void clear_logger() { this->m_logger = nullptr; }
		
		bool is_root() { return this->m_isroot; }
		
	private:
		FSMLevel m_minlevel = FSMLevel::Debug;
		FSMBasicStream *m_logger = nullptr;
		bool m_isroot = false;
	};
	
	FSMLogger& logger();
	
	FSMLogger& l(const std::string &name);
}

#endif /* defined(__MarXsCube__FSM__) */
