#ifndef CUBE_E_PHYSICSCOMMON_H
#define CUBE_E_PHYSICSCOMMON_H

#include "Common.h"

#include <vector>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#include <Bullet/btBulletDynamicsCommon.h>
#pragma GCC diagnostic pop

class PhysicsObject;

class ObjectCollideCache {
	public:
		std::vector<PhysicsObject *> objects;
		inline bool isempty() { return objects.empty(); }
		inline void insert(PhysicsObject *src) { objects.push_back(src); }
		inline void clear() { objects.clear(); }
		inline std::size_t size() { return objects.size(); }
};

inline btVector3 coord2bt(const CoordStruct& src) {
	return btVector3(btScalar(src.x*PHY_SCALE), btScalar(src.y*PHY_SCALE), btScalar(src.z*PHY_SCALE)); }

#endif