//
//  AcheronGCD.cpp
//  MarXsCube
//
//  Created by SeconDatke on 2/9/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "buildconf.h"
#include "AcheronGCD.h"

#ifdef CUBE_CONFIG_USE_DARWIN_LIBDISPATCH
#include <dispatch/dispatch.h>
#endif

#include "GritFoundation.h"
#include <vector>

namespace Acheron {

#ifdef CUBE_CONFIG_USE_DARWIN_LIBDISPATCH
	
	template <typename ReturnT>
	void async_dispatch(ThreadWorkerQueue& target_queue, std::function<ReturnT ()> dispatch_function, std::function<void (const ReturnT&)> callback) {
		dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^() {
			ReturnT t = dispatch_function();
			target_queue.add_worker({ [t, callback] () { callback(t); } });
		});
	}
	
#else
	
	void async_dispatch(ThreadWorkerQueue& target_queue, ThreadWorker::stdfunction_type dispatch_function, ThreadWorker::stdfunction_type callback) {
		dispatch_function();
		callback();
	}
	
#endif
	
template void async_dispatch(ThreadWorkerQueue& target_queue, std::function<std::vector<GPointType> ()> dispatch_function, std::function<void (const std::vector<GPointType>&)> callback);
	
}
