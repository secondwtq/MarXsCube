//
//  FSM.cpp
//  MarXsCube
//
//  Created by SeconDatke on 1/24/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "Common.h"

#include "FSM.h"

#include <stdio.h>
#include <iostream>
#include <unordered_map>
#include <memory>
#include <cassert>

#define cassert(condition, information) assert(condition)

const char * FSMLevelNames[static_cast<std::size_t>(FSMLevel::Lowest)+1] = {
	"HIGHEST", "FATAL", "ERROR", "WARNING",
	"DEBUG", "MESSAGE", "TRACE", "LOWEST",
};

namespace FSM {
	
namespace {
	
	std::unordered_map<std::string, std::shared_ptr<std::ofstream> > file_map;
	std::unordered_map<std::string, FSMLogger *> logger_map;
	
	FSMStdStream *stream_stdout = nullptr;
	FSMLogger *global_logger = nullptr;
	
	bool fsm_inited = false;
	
}
	
	void init() {
		cassert(fsm_inited == false, "FSM::init() - FSM inited but init() called.");
		
		stream_stdout = new FSMStdStream();
		stream_stdout->m_isroot = true;
		
		global_logger = new FSMLogger();
		global_logger->m_isroot = true;
		global_logger->set_logger(*stream_stdout);
		
		logger_map["root"] = global_logger;
		
		fsm_inited = true;
	}
	
	void dispose() {
		cassert(fsm_inited == true, "FSM::dispose() - FSM not inited but dispose() called.");
		cassert(file_map.size() == 0, "FSM::dispose() - file handle not released.");
		cassert(file_map.size() == 1, "FSM::dispose() - logger not released.");
		
		fsm_inited = false;
		
		global_logger->clear_logger();
		
		delete stream_stdout;
		stream_stdout = nullptr;
		delete global_logger;
		global_logger = nullptr;
	}
	
	bool inited() { return fsm_inited; }
	
	FSMLogger &logger() {
		cassert(fsm_inited == true, "FSM::logger() - FSM not inited but trying to get root logger.");
		return *global_logger;
	}
	
	FSMLogger &get_logger(const std::string& name) {
		if (logger_map.find(name) != logger_map.end()) {
			return *logger_map.at(name);
		} else {
			FSMLogger *logger = new FSMLogger;
			logger_map.insert({ name, logger });
			return *logger;
		}
	}
	
	void dispose_logger(const std::string &name) {
		if (logger_map.find(name) != logger_map.end()) {
			FSMLogger *logger = logger_map.at(name);
			logger_map.erase(name);
			delete logger;
		} else {
			cassert(logger_map.find(name) != logger_map.end(),
					"FSM::dispose_logger() - trying to dispose a logger that not exists.");
		}
	}
	
	FSMLogger::FSMLogger() {
		// init proxies
		for (std::size_t i = (std::size_t)FSMLevel::Highest; i <= (std::size_t)FSMLevel::Lowest; i++)
			this->m_proxies.push_back(std::unique_ptr<FSMLoggerProxy>(new FSMLoggerProxy(*this, static_cast<FSMLevel>(i))));
	}
	
	void FSMLogger::log(const char *src) {
		cassert(this->m_logger != nullptr, "FSM::FSMLogger::log - logger has no stream but trying to log.");
		this->m_logger->writeln(src);
	}
	
	void FSMLogger::log_noendl(const char *src) {
		cassert(this->m_logger != nullptr, "FSM::FSMLogger::log_noendl - logger has no stream but trying to log.");
		this->m_logger->write(src);
	}
	
	FSMLoggerProxy& FSMLogger::operator[](FSMLevel level) {
		return *this->m_proxies[static_cast<std::size_t>(level)]; }
	
	void FSMStdStream::close() { }

	void FSMStdStream::write(const char *src) {
		printf("%s", src); }
	
	void FSMStdStream::writeln(const char *src) {
		printf("%s\n", src); }
	
	void FSMCompositeStream::close() {
		for (auto stream : this->m_loggers)
			stream->close(); }
	
	void FSMCompositeStream::write(const char *src) {
		for (auto stream : this->m_loggers)
			stream->write(src); }
	
	void FSMCompositeStream::writeln(const char *src) {
		for (auto stream : this->m_loggers)
			stream->writeln(src); }
	
	FSMCompositeStream& FSMCompositeStream::add_logger(FSMBasicStream &src) {
		this->m_loggers.push_back(&src);
		return *this; }
	
	void FSMFileStream::close() {
		
	}
	
	void FSMFileStream::write(const char *src) {
		
	}
	
	void FSMFileStream::writeln(const char *src) {
		
	}
}