#ifndef CUBE_E_PHYSICS_H
#define CUBE_E_PHYSICS_H

#include "Common.h"
#include <vector>
#include <string.h>
#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>
#include "Config.h"
#include "PhysicsCommon.h"
#include "PhysicsGeneral.h"
#include "ObjectArray.h"
#include "Generic.h"

class ConfigManger;
class Abs_Abstract;

enum PhysicsShapes {
	BOX,
	SPHERE,
	CYLINDER,
	COMPOUND
};

class PhysicsShapeType {
	public:
		PhysicsShapes Type;
		float mass = 1.0f;
		float restitution = 0.0f;
		float friction = 0.5f;

		virtual bool LoadFromConfig(ConfigManger &manger, luabridge::LuaRef ShapeRef);
		virtual btCollisionShape *createShape() = 0;
        virtual ~PhysicsShapeType() { }
};

class PhysicsShapeTypeBox : public PhysicsShapeType {
	public:
		CoordStruct size = CoordStruct(0, 0, 0);

		bool LoadFromConfig(ConfigManger &manger, luabridge::LuaRef ShapeRef);
		btCollisionShape *createShape();
};

class PhysicsObjectType {
	public:
		bool EnablePhysics = false;
		float mass = 1.0f;
		luabridge::LuaRef ShapeRef;
		PhysicsShapeType *ShapeType;
		btVector3 offset = btVector3(btScalar(0), btScalar(0), btScalar(0));
		btVector3 angleFact = btVector3(btScalar(1), btScalar(1), btScalar(1));

		PhysicsObjectType() : ShapeRef(EventManger::GetInstance()._state->State) { }
		bool LoadFromConfig(ConfigManger &manger, luabridge::LuaRef ParentRef);
};

class PhysicsShape {
	public:
		PhysicsShapeType *Type;
};

class PhysicsObject {
	public:
		static ObjectArray<PhysicsObject> Array;

		bool EnablePhysics = false;
		bool spawned = false;
		PhysicsObjectType *Type = nullptr;
		btVector3 location;
		btDefaultMotionState *MotionState = nullptr;
		PhysicsShape *shape = nullptr;
		btCollisionShape *_shape = nullptr;
		btRigidBody *body = nullptr;
		Abs_Abstract *attachedToObject = nullptr;
		btVector3 offset = btVector3(btScalar(0), btScalar(0), btScalar(0));
	
		bool isEmpty = false;
		bool isCell = false;

		ObjectCollideCache collCache;

		~PhysicsObject();

		void SpawnAt(const CoordStruct &loc);

		void setTransformCallback(const btTransform &centerOfMassWorldTrans);

		void setDirection(int degree);

		void setLocation(const CoordStruct &loc);

		void applyImpulse(const Float3D &impluse, const CoordStruct &rel_pos);

		void setLinearVelocity(const Float3D &velocity);

		void setVelocity(double velocity);

		double getVelocity();

		void setMainRotationVelocity(double velocity);

		Float3D getLinearVelocity();

		void applyForce(const Float3D &force, const CoordStruct &rel_pos);

		void applyForce_Central(const Float3D &force);

		void applyForce_Directional(float force, const CoordStruct &rel_pos);

		void applyCentralForce_Directional(float force);

		void disableResponse();

		void setToStatic();

		void setToKinematic();

		void activate();

		double getVerticalVelocity();

		void applyCentralForce_Vertical(float force);

		void applyCentralImpulse(const Float3D &impluse);

		void applyCentralImpulse_Directional(float impluse);

		void applyCentralImpulse_Vertical(float impluse);

		double getMainRotationVelocity();

		void clearVerticalVelocity();

		void setDirectionTo(int degree);

		bool checkCollide();

		bool checkCollide_();

		PhysicsObject(bool empty) : isEmpty(empty) {LOGFUNC; }

		PhysicsObject(Abs_Abstract *parent) : attachedToObject(parent) {LOGFUNC;
			PhysicsObject::Array.Array.push_back(this);
			// std::cout << "CubeCore: PhysicsObject::Constructing ..." << std::endl;
		}
};

#include "Config.h"
#include "Abstract.h"

#endif