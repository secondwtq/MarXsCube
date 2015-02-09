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
#include <functional>

namespace Acheron {

	void async_dispatch(ThreadWorkerQueue& target_queue, ThreadWorker::stdfunction_type dispatch_function, ThreadWorker::stdfunction_type callback);
	
}

#endif /* defined(__MarXsCube__AcheronGCD__) */
