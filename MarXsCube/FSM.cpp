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

namespace FSM {
	
namespace {
	std::unordered_map<std::string, std::shared_ptr<std::ofstream> > file_map;
	
	FSMStdStream *stream_stdout = nullptr;
	FSMLogger *global_logger = nullptr;
}
	
	void init() {
		stream_stdout = new FSMStdStream();
		stream_stdout->m_isroot = true;
		
		global_logger = new FSMLogger();
		global_logger->m_isroot = true;
		global_logger->set_logger(*stream_stdout);
	}
	
	void dispose() {
		cassert(file_map.size() == 0, "FSM::dispose() - file handle not released.");
		
		global_logger->clear_logger();
		
		delete stream_stdout;
		stream_stdout = nullptr;
		delete global_logger;
		global_logger = nullptr;
	}
	
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