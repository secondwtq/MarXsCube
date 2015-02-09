//
//  AcheronGCD.h
//  MarXsCube
//
//  Created by SeconDatke on 2/9/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef __MarXsCube__AcheronGCD__
#define __MarXsCube__AcheronGCD__

#include "Config.h"
#include "Acheron.h"
#include <mutex>
#include <queue>
#include <functional>

namespace Acheron {
	
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

	template <typename ReturnT>
	void async_dispatch(ThreadWorkerQueue& target_queue, std::function<ReturnT ()> dispatch_function, std::function<void (const ReturnT&)> callback);
	
}

#endif /* defined(__MarXsCube__AcheronGCD__) */
