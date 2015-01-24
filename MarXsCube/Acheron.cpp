//
//  Acheron.cpp
//  MarXsCube
//
//  Created by SeconDatke on 1/24/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "Common.h"

#include "Acheron.h"

#include <thread>
#include <functional>

using namespace std;

void Acheron::AcheronBase::start() {
	this->m_running = true;
	this->m_thread = thread(&::Acheron::AcheronBase::function, this);
}

void Acheron::AcheronBase::function() {
	while (true) {
		unique_lock<mutex> lk(this->m_mutex);
		if (this->m_sync) this->m_cond.wait(lk, [this] { return this->m_cycle; });
		if (!this->m_running) return lk.unlock();
		this->m_target();
		if (this->m_running) this->m_cycle = false;
		else return lk.unlock();
		lk.unlock();
	}
}