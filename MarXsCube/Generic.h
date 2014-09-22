//
//  Generic.h
//  MarXsCube
//
//  Created by SeconDatke on 14-9-22.
//  Copyright (c) 2014年 MarXsCube Staff. All rights reserved.
//

#ifndef __MarXsCube__Generic__
#define __MarXsCube__Generic__

#include "Common.h"
#include "Session.h"
#include "ObjectManger.h"

class Generic {
public:
	
	static inline Session *Session() {
		return Generic::session;
	}
	
	static inline RenderLayerManger *RenderLayerManger() {
		return Generic::render_layer_manger;
	}
	
	static inline void Init_Session() {
		Generic::session = new class Session();
	}
	
	static inline void Init_RenderLayerManger() {
		Generic::render_layer_manger = new class RenderLayerManger();
	}
 
	static inline void RemoveObject(Abs_Abstract *src) {
		Generic::Session()->removeObject(src);
	}
	
public:
	static class Session *session;
	static class RenderLayerManger *render_layer_manger;
};

#endif /* defined(__MarXsCube__Generic__) */
