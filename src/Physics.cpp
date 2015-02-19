#include "Common.h"
#include "MotionStateTest.h"
#include "Config.h"
#include "LuaUtils.h"
#include "PhysicsCheck.h"
#include "PhysicsGeneral.h"

#include "Physics.h"

using namespace std;
using namespace luabridge;

ObjectArray<PhysicsObject> PhysicsObject::Array = ObjectArray<PhysicsObject>();

bool PhysicsShapeType::LoadFromConfig(ConfigManger &manger, luabridge::LuaRef ShapeRef) {LOGFUNC;
	restitution = ShapeRef["restitution"];
	friction = ShapeRef["friction"];
	return true;
}

bool PhysicsShapeTypeBox::LoadFromConfig(ConfigManger &manger, luabridge::LuaRef ShapeRef) {LOGFUNC;
	std::cout << "CubeCore: PhysicsShapeTypeBox::LoadFromConfig - Loading ..." << std::endl;
	PhysicsShapeType::LoadFromConfig(manger, ShapeRef);
	LuaRef sizeRef = ShapeRef["size"];
	size = CoordStruct((double)sizeRef["x"]*PHY_SCALE, (double)sizeRef["y"]*PHY_SCALE, (double)sizeRef["z"]*PHY_SCALE);
	return true;
}

bool PhysicsShapeTypeCompBox::LoadFromConfig(ConfigManger &manger, luabridge::LuaRef ShapeRef) {LOGFUNC;
	std::cout << "CubeCore: PhysicsShapeTypeCompBox::LoadFromConfig - Loading ..." << std::endl;
	PhysicsShapeTypeBox::LoadFromConfig(manger, ShapeRef);
	LuaRef offsetRef = ShapeRef["offset_comp"];
	this->offset = CoordStruct((double)offsetRef["x"]*PHY_SCALE, (double)offsetRef["y"]*PHY_SCALE, (double)offsetRef["z"]*PHY_SCALE);
	return true;
}

btCollisionShape *PhysicsShapeTypeBox::createShape() {LOGFUNC;
	return new btBoxShape(btVector3(btScalar(size.x), btScalar(size.y), btScalar(size.z)));
}

btCollisionShape *PhysicsShapeTypeCompBox::createShape() {LOGFUNC;
	btCollisionShape *inner_shape = new btBoxShape(btVector3(btScalar(size.x), btScalar(size.y), btScalar(size.z)));
	btCompoundShape* compound = new btCompoundShape();
	btTransform local_trans;
	local_trans.setIdentity();
	local_trans.setOrigin(btVector3(btScalar(offset.x), btScalar(offset.y), btScalar(offset.z)));
	compound->addChildShape(local_trans, inner_shape);
	return compound;
}

btCollisionShape *PhysicsShapeTypeMeshStatic::createShape() {
	bool use_quantization = true;
	return new btBvhTriangleMeshShape(this->_mesh, use_quantization);
}

bool PhysicsObjectType::LoadFromConfig(ConfigManger &manger, LuaRef ParentRef) {LOGFUNC;
	std::cout << "CubeCore: PhysicsObjectType::LoadFromConfig - Loading " << std::endl;
	LuaRef PhysicsRef = ParentRef["physics"];
	EnablePhysics = PhysicsRef["enabled"];
	ShapeRef = PhysicsRef["shape"];

	if (EnablePhysics) {
		mass = PhysicsRef["mass"];
		LuaRef offsetRef = PhysicsRef["offset"];
		offset = btVector3(btScalar((double)offsetRef["x"]*PHY_SCALE), btScalar((double)offsetRef["y"]*PHY_SCALE), btScalar((double)offsetRef["z"]*PHY_SCALE));

		LuaRef angleFactRef = PhysicsRef["angle_factor"];
		angleFact = btVector3(btScalar(angleFactRef["x"]), btScalar(angleFactRef["y"]), btScalar(angleFactRef["z"]));
		if (ShapeRef["type"] == "BOX") 
			ShapeType = new PhysicsShapeTypeBox();
		else if (ShapeRef["type"] == "COMPBOX")
			ShapeType = new PhysicsShapeTypeCompBox();
		ShapeType->LoadFromConfig(manger, ShapeRef);
	}

	std::cout << " finished ..."  << std::endl;
	return true;
}

PhysicsObject::~PhysicsObject() {LOGFUNC;
	delObject_all(this, PhysicsObject::Array.Array);
	if (spawned) Generic::PhysicsGeneral()->dynaWorld->removeRigidBody(body);
	delete body;
	delete MotionState;
	delete shape;
	if (attachedToObject) {
		delete attachedToObject->collSphere;
	}
}

void PhysicsObject::SpawnAt(const CoordStruct &loc) {LOGFUNC;
	if ((!attachedToObject) || attachedToObject->EnablePhysics != false) {
		offset = Type->offset;
		_shape = Type->ShapeType->createShape();
		btTransform trans = btTransform::getIdentity();
		btVector3 localInertia(0, 0, 0);
		btVector3 _loc = btVector3(btScalar(loc.x*PHY_SCALE), btScalar(loc.y*PHY_SCALE), btScalar(loc.z*PHY_SCALE))+offset;
		trans.setOrigin(_loc);
		if (Type->mass != 0.f) _shape->calculateLocalInertia(Type->mass,localInertia);
		MotionState = new MoState(trans, this);
		btRigidBody::btRigidBodyConstructionInfo info(Type->mass, MotionState, _shape, localInertia);
		body = new btRigidBody(info);
		body->setRestitution(btScalar(Type->ShapeType->restitution));
		body->setFriction(btScalar(Type->ShapeType->friction));
		body->setAngularFactor(Type->angleFact);
		body->setUserPointer(this);
		Generic::PhysicsGeneral()->dynaWorld->addRigidBody(body);

		if (attachedToObject)
			attachedToObject->collSphere = new btSphereShape(btScalar(attachedToObject->rCollSphere));

		spawned = true;
		// body->activate();
	}
}

void cubeTickCallback(btDynamicsWorld *world, btScalar timeStep) {
	for (auto i : PhysicsObject::Array.Array)
		if (!i->collCache.isempty())
			i->collCache.clear();

	int numManifolds = world->getDispatcher()->getNumManifolds();
	for (int i = 0; i<numManifolds; i++) {
		btPersistentManifold* contactManifold =  world->getDispatcher()->getManifoldByIndexInternal(i);
		PhysicsObject *pA = (PhysicsObject *)(static_cast<const btCollisionObject*>(contactManifold->getBody0())->getUserPointer()),
						*pB = (PhysicsObject *)(static_cast<const btCollisionObject*>(contactManifold->getBody1())->getUserPointer());
		if ((!pA) || (!pB)) continue;
		if ((pA->isEmpty == false) && (pB->isEmpty == false)) {
				pA->collCache.insert(pA);
				pA->collCache.insert(pB);
		}

	}
}

bool PhysicsObject::checkCollide() {
	bool ret = true;
	btTransform trans = body->getWorldTransform();
	auto state = new btDefaultMotionState(trans);
	btRigidBody::btRigidBodyConstructionInfo info(0, state, _shape, btVector3(0, 0, 0));
	auto TestBody = new btRigidBody(info);
	ContactSensorCallback_CellCheck_ callback(*TestBody, &ret, this->attachedToObject);
	Generic::PhysicsGeneral()->dynaWorld->contactTest(TestBody, callback);
	delete TestBody;
	delete state;
	if (ret == false) return true;
	return false;
}

bool PhysicsObject::checkCollide_() {
	// printf("%d\n", collCache.size());
	return !(collCache.isempty());
}

void PhysicsObject::setLocation(const CoordStruct &loc) {LOGFUNC;
	btTransform tr = body->getCenterOfMassTransform();
	tr.setOrigin(btVector3(btScalar(loc.x*PHY_SCALE), btScalar(loc.y*PHY_SCALE), btScalar(loc.z*PHY_SCALE))+offset);
	body->setCenterOfMassTransform(tr);
	setTransformCallback(tr);
}

void PhysicsObject::applyImpulse(const Float3D &impluse, const CoordStruct &rel_pos) {LOGFUNC;
	body->activate();
	body->applyImpulse(btVector3(btScalar(impluse.x), btScalar(impluse.y), btScalar(impluse.z)), btVector3(btScalar(rel_pos.x), btScalar(rel_pos.y), btScalar(rel_pos.z)));
}

Float3D PhysicsObject::getLinearVelocity() {
	auto ret = body->getLinearVelocity();
	return Float3D(ret.x(), ret.y(), ret.z());
}

void PhysicsObject::setVelocity(double velocity) {
	auto v = body->getLinearVelocity();
	double r = torad(attachedToObject->getMainRotation());
	auto vel = Float3D(cos(r) * velocity, -sin(r) * velocity, v.z());
	setLinearVelocity(vel);
}

double PhysicsObject::getVelocity() {
	auto r = body->getLinearVelocity();
	double rad = torad(attachedToObject->getMainRotation());
	return (cos(rad)*r.x() - sin(rad)*r.y());
}

double PhysicsObject::getMainRotationVelocity() {
	auto r = body->getAngularVelocity();
	return r.z();
}

void PhysicsObject::setMainRotationVelocity(double velocity) {
	auto r = body->getAngularVelocity();
	r.setZ(btScalar(velocity));
	body->setAngularVelocity(r);
}

void PhysicsObject::setLinearVelocity(const Float3D &velocity) {
	body->activate();
	body->setLinearVelocity(btVector3(btScalar(velocity.x), btScalar(velocity.y), btScalar(velocity.z)));
}

void PhysicsObject::applyForce(const Float3D &force, const CoordStruct &rel_pos) {
	body->applyForce(btVector3(btScalar(force.x), btScalar(force.y), btScalar(force.z)), btVector3(btScalar(rel_pos.x), btScalar(rel_pos.y), btScalar(rel_pos.z)));
}

void PhysicsObject::applyForce_Central(const Float3D &force) {
	body->applyCentralForce(btVector3(btScalar(force.x), btScalar(force.y), btScalar(force.z)));
}

void PhysicsObject::applyCentralForce_Directional(float force) {
	double rad = torad(attachedToObject->getMainRotation());
	auto foc = Float3D(cos(rad) * force, -sin(rad) * force, 0);
	applyForce_Central(foc);
}

void PhysicsObject::applyCentralImpulse_Directional(float impluse) {
	double rad = torad(attachedToObject->getMainRotation());
	auto imp = Float3D(cos(rad) * impluse, -sin(rad) * impluse, 0);
	applyCentralImpulse(imp);
}

double PhysicsObject::getVerticalVelocity() {
	auto r = body->getLinearVelocity();
	double rad = torad(attachedToObject->getMainRotation());
	return (sin(rad)*r.x() + cos(rad)*r.y());
}

void PhysicsObject::clearVerticalVelocity() {
	auto v = body->getLinearVelocity();
	double r = torad(attachedToObject->getMainRotation());
	auto vel = Float3D(v.x()+sin(r)*v.x(), v.y()-cos(r)*v.y(), v.z());
	setLinearVelocity(vel);
}

void PhysicsObject::applyCentralForce_Vertical(float force) {
	auto foc = Float3D(-forward_vec.y * force, forward_vec.x * force, 0);
	applyForce_Central(foc); }

void PhysicsObject::applyCentralImpulse(const Float3D &impluse) {
	body->activate();
	body->applyCentralImpulse(btVector3(btScalar(impluse.x), btScalar(impluse.y), btScalar(impluse.z)));
}

void PhysicsObject::applyCentralImpulse_Vertical(float impluse) {
	auto imp = Float3D(-forward_vec.y * impluse, forward_vec.x * impluse, 0);
	applyCentralImpulse(imp); }

void PhysicsObject::applyImpulse_Vertical(float impluse, const Float3D& rel_pos) {
	auto imp = Float3D(-forward_vec.y * impluse, forward_vec.x * impluse, 0);
	this->body->activate();
	this->body->applyImpulse(btVector3(btScalar(imp.x), btScalar(imp.y), btScalar(imp.z)), btVector3(btScalar(rel_pos.x*PHY_SCALE), btScalar(rel_pos.y*PHY_SCALE), btScalar(rel_pos.z*PHY_SCALE))); }

void PhysicsObject::applyForce_Directional(float force, const CoordStruct &rel_pos) {
	double rad = torad(attachedToObject->getMainRotation());
	auto foc = Float3D(cos(rad) * force, -sin(rad) * force, 0);
	applyForce(foc, rel_pos);
}

void PhysicsObject::applyTorqueImpulse_Initial(const CoordStruct& torque) {
	body->applyTorqueImpulse(btVector3(btScalar(torque.x), btScalar(torque.y), btScalar(torque.z)));
}

void PhysicsObject::activate() {
	body->activate();
}

// the rad space of Script Environment is 0 -> 2pi
//	but in Bullet, it is -pi -> pi
double PhysicsObject::getMainRotation() {
	if (this->main_rotation < 0) {
		this->main_rotation += 2*PI;
	}
	return this->main_rotation;
}

void PhysicsObject::setTransformCallback(const btTransform &centerOfMassWorldTrans) {LOGFUNC;
	if (attachedToObject && attachedToObject->EnablePhysics) {
		attachedToObject->setLocation(CoordStruct((int)((centerOfMassWorldTrans.getOrigin().getX()-offset.x())/PHY_SCALE),
												  (int)((centerOfMassWorldTrans.getOrigin().getY()-offset.y())/PHY_SCALE),
												  (int)((centerOfMassWorldTrans.getOrigin().getZ()-offset.z())/PHY_SCALE)));
		btMatrix3x3 rot = centerOfMassWorldTrans.getBasis();
		float x, y, z;
		rot.getEulerZYX(z, y, x);
		attachedToObject->setMainRotation(-z * (180.0 / PI));
		this->main_rotation = z;

		this->forward_vec = { rot[1][1], -rot[0][1], rot[2][1] };
	}
}

void PhysicsObject::setDirection(int degree) {LOGFUNC;
	btTransform tr = body->getCenterOfMassTransform();
	btQuaternion quat = tr.getRotation(), rotquat = rotquat.getIdentity();
	rotquat.setEulerZYX(degree / (180.0 / PI), 0, 0);
	quat = quat * rotquat;
	tr.setRotation(quat);
	body->setCenterOfMassTransform(tr);
	setTransformCallback(tr);
}

void PhysicsObject::setDirectionTo(int degree) {LOGFUNC;
	btTransform tr = body->getCenterOfMassTransform();
	btQuaternion rotquat = rotquat.getIdentity();
	rotquat.setEulerZYX(degree / (180.0 / PI), 0, 0);
	tr.setRotation(rotquat);
	body->setCenterOfMassTransform(tr);
}

void PhysicsObject::disableResponse() {LOGFUNC;
	body->setCollisionFlags(body->getCollisionFlags() || btCollisionObject::CollisionFlags::CF_NO_CONTACT_RESPONSE);
}

void PhysicsObject::setToStatic() {LOGFUNC;
	Generic::PhysicsGeneral()->dynaWorld->removeRigidBody(body);
	body->setLinearVelocity(btVector3(0, 0, 0));
	body->setAngularVelocity(btVector3(0, 0, 0));
	body->setMassProps(0, btVector3(0,0,0));
	body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CollisionFlags::CF_STATIC_OBJECT);
	body->setActivationState(DISABLE_DEACTIVATION);
	Generic::PhysicsGeneral()->dynaWorld->addRigidBody(body);
}

void PhysicsObject::setToKinematic() {LOGFUNC;
	Generic::PhysicsGeneral()->dynaWorld->removeRigidBody(body);
	body->setLinearVelocity(btVector3(0, 0, 0));
	body->setAngularVelocity(btVector3(0, 0, 0));
	body->setMassProps(0, btVector3(0,0,0));
	body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CollisionFlags::CF_KINEMATIC_OBJECT);
	body->setActivationState(DISABLE_DEACTIVATION);
	Generic::PhysicsGeneral()->dynaWorld->addRigidBody(body);
}



MoState::MoState(const btTransform& startTrans, PhysicsObject *object, const btTransform& centerOfMassOffset)
	: btDefaultMotionState(startTrans, centerOfMassOffset), object_ptr(object) {LOGFUNC; }

void MoState::getWorldTransform(btTransform &centerOfMassWorldTrans) const {LOGFUNC;
	btDefaultMotionState::getWorldTransform(centerOfMassWorldTrans);
}

void MoState::setWorldTransform(const btTransform &centerOfMassWorldTrans) {LOGFUNC;
	// object_ptr->setLocation(CoordStruct((int)(centerOfMassWorldTrans.getOrigin().getX()), (int)(centerOfMassWorldTrans.getOrigin().getY()), (int)(centerOfMassWorldTrans.getOrigin().getZ())));
	object_ptr->setTransformCallback(centerOfMassWorldTrans);
	btDefaultMotionState::setWorldTransform(centerOfMassWorldTrans);
}

ContactSensorCallback_CellCheck::ContactSensorCallback_CellCheck(btRigidBody& tgtBody , Abs_Cell *_cell)
		: btCollisionWorld::ContactResultCallback(), body(tgtBody), cell(_cell) {
			cell->__occupiedByObject = nullptr;
		}

btScalar ContactSensorCallback_CellCheck::addSingleResult(btManifoldPoint& cp,
	const btCollisionObjectWrapper* colObj0,int partId0,int index0,
	const btCollisionObjectWrapper* colObj1,int partId1,int index1) {
	btVector3 pt; // will be set to point of collision relative to body
	Abs_Abstract *targetPtr = nullptr;
	if(colObj0->m_collisionObject==&body) {
		pt = cp.m_localPointA;
		targetPtr = ((PhysicsObject *)((btCollisionObject *)colObj1->m_collisionObject->getUserPointer()))->attachedToObject;
	} else {
		assert(colObj1->m_collisionObject==&body && "body does not match either collision object");
		pt = cp.m_localPointB;
		targetPtr = ((PhysicsObject *)((btCollisionObject *)colObj0->m_collisionObject->getUserPointer()))->attachedToObject;
	}
	cell->__occupiedByObject = targetPtr;
	return 0;
}