//
//  TilerObject.h
//  MarXsCube
//
//  Created by SeconDatke on 2/5/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef __MarXsCube__TilerObject__
#define __MarXsCube__TilerObject__

#include "PhysicsCommon.h"

class TilerObject {
public:
	
private:
	
	btTriangleMesh *m_bmesh = nullptr;
	btBvhTriangleMeshShape *m_bshape = nullptr;
	btTriangleInfoMap *m_btri_map = nullptr;
	btRigidBody *m_bbody = nullptr;
	btDefaultMotionState *m_bmotion_state = nullptr;
	
};

#endif /* defined(__MarXsCube__TilerObject__) */
