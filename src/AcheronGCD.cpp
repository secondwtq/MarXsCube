//
//  AcheronGCD.cpp
//  MarXsCube
//
//  Created by SeconDatke on 2/9/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "AcheronGCD.h"

#include <dispatch/dispatch.h>

namespace Acheron {

#ifdef CUBE_CONFIG_USE_DARWIN_LIBDISPATCH
	
	void async_dispatch(ThreadWorkerQueue& target_queue, ThreadWorker::stdfunction_type dispatch_function, ThreadWorker::stdfunction_type callback) {
		dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^() {
			dispatch_function();
			target_queue.add_worker({ callback });
		});
	}
	
#else
	
	void async_dispatch(ThreadWorkerQueue& target_queue, ThreadWorker::stdfunction_type dispatch_function, ThreadWorker::stdfunction_type callback) {
		dispatch_function();
		callback();
	}
	
#endif
	
}
