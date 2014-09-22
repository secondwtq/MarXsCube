#ifndef CUBE_E_PHYSICSCOMMON_H
#define CUBE_E_PHYSICSCOMMON_H

#include "Common.h"

#include <vector>

#include <Bullet/btBulletDynamicsCommon.h>

class PhysicsObject;

class ObjectCollideCache {
	public:
		std::vector<PhysicsObject *> objects;
		inline bool isempty() { return objects.empty(); }
		inline void insert(PhysicsObject *src) { objects.push_back(src); }
		inline void clear() { objects.clear(); }
		inline std::size_t size() { return objects.size(); }
};

#endif