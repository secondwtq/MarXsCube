//
//  Acheron.h
//  MarXsCube
//
//  Created by SeconDatke on 1/24/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef __MarXsCube__Acheron__
#define __MarXsCube__Acheron__

#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <queue>

namespace Acheron {
	
	inline void foo_placeholder() { }
	
	enum SYNCSTATE { UNSYNCED = 0, SYNCED = 1, SYNCED_NONAUTO = 2 };
	
	class AcheronBase {
	public:
		
		typedef void (function_type)();
		typedef std::function<function_type> std_function_type;
		
		AcheronBase();
		
		AcheronBase(std_function_type target) : m_target(target) { }
		
		inline void set_target(std_function_type target) {
			this->m_target = target; }
		
		inline void start(SYNCSTATE synced) { this->start(static_cast<bool>(synced)); this->m_auto_lock = (synced != SYNCED_NONAUTO); }
		inline void invoke() { if (this->m_running) { this->m_mutex.unlock(); this->m_cycle = true; this->m_cond.notify_all(); } }
		inline void pause() { if (this->m_running) this->m_mutex.lock(); }
		inline void invoke_and_stop() { this->_invoke(); this->stop(); }
		
		void start();
		inline void start(bool synced) { this->sync(synced); this->start(); }
		inline void sync(bool value) { this->m_sync = value; }
		
		inline void _invoke() {
//						if (this->m_mutex.try_lock())
//							this->m_mutex.unlock();
//						else {
//							this->m_mutex.lock();
//							this->m_mutex.unlock();
//						}
			this->m_mutex.unlock(); // hack, only applies to Bullet & Silcon
				// do you think threading library of C++11 is tooooo simple to use?
			this->m_cycle = true; this->m_cond.notify_all();
		}
		inline void stop() { this->m_running = false; this->m_thread.join(); }
		
	private:
		
		void function();
		
		std_function_type m_target = &::Acheron::foo_placeholder;
		
		std::thread m_thread;
		std::mutex m_mutex;
		std::condition_variable m_cond;
		
		bool m_sync = false;
		bool m_running = false;
		bool m_cycle = false;
		
		bool m_auto_lock = true;
	};
	
	class ThreadWorker {
	public:
		
		typedef std::function<void ()> stdfunction_type;
		
		ThreadWorker(const stdfunction_type& function)
			: m_worker(function) { }
		void dispatch() { this->m_worker(); }
		
	private:
		stdfunction_type m_worker;
	};
	
	class ThreadWorkerQueue {
	public:
		
		void update() {
			this->m_queue_mutex.lock();
			while (!this->m_queue.empty()) {
				this->m_queue.front().dispatch();
				this->m_queue.pop();
			}
			this->m_queue_mutex.unlock();
		}
		
		void add_worker(const ThreadWorker& worker) {
			this->m_queue_mutex.lock();
			this->m_queue.push(worker);
			this->m_queue_mutex.unlock();
		}
		
	private:
		std::queue<ThreadWorker> m_queue;
		std::mutex m_queue_mutex;
	};
	
}

#endif /* defined(__MarXsCube__Acheron__) */
