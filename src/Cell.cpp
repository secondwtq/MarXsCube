#include "Cell.h"
#include "ObjectManger.h"
#include "SFML.h"
#include "Physics.h"
#include "PhysicsCheck.h"
#include "Techno.h"
using namespace sf;

CoordStruct Abs_Cell::GetCoord() const {//LOGFUNC;
	static CoordStruct t(0, 0, 0);
	return t;
}

Abs_Cell::Abs_Cell(int x, int y, bool Special) :
	Abs_Abstract(nullptr), LocCell(x, y), isSpecial(Special) {
	LocCoord = CoordStruct(LocCell.x * DIVS, LocCell.y * DIVS, height);

	_coordObj[0] = -LocCell.x * DIVS;
	_coordObj[1] = -LocCell.y * DIVS;
	_coordObj[2] = -height;
	_coordObj[3] = 1;

	occTest_physhape = new btBoxShape(btVector3(btScalar(DIVS/2), btScalar(DIVS/2), btScalar(DIVS/2)));
	btTransform trans = btTransform::getIdentity();
	trans.setOrigin(btVector3(btScalar(LocCoord.x), btScalar(LocCoord.y), btScalar(DIVS/2)));
	occTest_mo = new btDefaultMotionState(trans);
	btRigidBody::btRigidBodyConstructionInfo info(0, occTest_mo, occTest_physhape, btVector3(0, 0, 0));

	trans = btTransform::getIdentity();
	trans.setOrigin(btVector3(btScalar(LocCoord.x), btScalar(LocCoord.y), btScalar(0)));
	sphereTest_mo = new btDefaultMotionState(trans);
	// occTestBody = new btRigidBody(info);
}

void Abs_Cell::Render() {

}

bool Abs_Cell::initialPerformOccTest(int xoff, int yoff, Abs_Object *Obj, int hei) {
	btVector3 start = btVector3(btScalar(xoff), btScalar(yoff), btScalar(hei));
	btVector3 end = btVector3(btScalar(xoff), btScalar(yoff), btScalar(hei+DIVS));
	btCollisionWorld::ClosestRayResultCallback callback(start, end);
	Generic::PhysicsGeneral()->dynaWorld->rayTest(start, end, callback);
	if (callback.hasHit() && ((PhysicsObject *)((btCollisionObject *)callback.m_collisionObject->getUserPointer()))->attachedToObject != Obj) return true;
	return false;
}

void Abs_Cell::Update() { }

Abs_Abstract *Abs_Cell::_initialPerformOccTest(int xoff, int yoff, int hei) {
	btVector3 start = btVector3(btScalar(xoff), btScalar(yoff), btScalar(hei));
	btVector3 end = btVector3(btScalar(xoff), btScalar(yoff), btScalar(hei+DIVS));
	btCollisionWorld::ClosestRayResultCallback callback(start, end);
	Generic::PhysicsGeneral()->dynaWorld->rayTest(start, end, callback);
	if (callback.hasHit()) return ((PhysicsObject *)((btCollisionObject *)callback.m_collisionObject->getUserPointer()))->attachedToObject;
	return nullptr;
}

bool Abs_Cell::PassableTo(Abs_Object *Obj) {//LOGFUNC;
	if (__occupiedByObject && __occupiedByObject != Obj) return false;
	if (occupiedByObject && occupiedByObject != Obj) return false;


	if (Obj->repathcache.inside(this)) {
		// if (Obj->repathcache.size() >= 3) Obj->repathcache.clear();
		return false;
	}

	if (Obj->useCollSphere) {
		bool ret = true;
		btRigidBody::btRigidBodyConstructionInfo info(0, sphereTest_mo, Obj->collSphere, btVector3(0, 0, 0));
		auto TestBody = new btRigidBody(info);
		ContactSensorCallback_CellCheck_ callback(*TestBody, &ret, Obj);
        //  TODO: modified fm. MarKsCube_Mac
		Generic::PhysicsGeneral()->dynaWorld->contactTest(TestBody, callback);
		delete TestBody;
		if (ret == false) return false;
	}
	
	return true;
}