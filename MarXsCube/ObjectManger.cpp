#include "ObjectManger.h"
#include "Map.h"

#include "Type_Anim.h"
#include "Anim.h"

#include "Config.h"
#include "Session.h"
#include "Physics.h"

#include <fstream>
#include <Bullet/btBulletDynamicsCommon.h>

ObjectManger *ObjectManger::instance = new ObjectManger();
TestManger *TestManger::instance = new TestManger();
TextureManger *TextureManger::instance = new TextureManger();
RenderLayerManger *RenderLayerManger::instance = new RenderLayerManger();

int TextureManger::t(int x) {LOGFUNC; printf("%d\n", x); return 10; }
TextureAtlas *TextureManger::getTexture(const string &name) {LOGFUNC; return TextureHashs[name]; }
TextureAtlas &TextureManger::getTexture_(string &name) {LOGFUNC; return *TextureHashs[name]; }

void RenderLayer::Update() {LOGFUNC;
	sort_Objects();
	for (size_t i = 0; i < Objects.size(); i++)
		Objects[i]->Render();
}

void RenderLayerManger::RemoveObject(Abs_Abstract *src) {LOGFUNC;
	// for(auto l : Layers) {
	// 	printf("removing ...\n");
	// 	l.RemoveObject(src);
	// }
	for(size_t i = 0; i < RenderLayerType::Count; i++)
		Layers[i].RemoveObject(src);
}

void TestManger::initTest() {LOGFUNC;
	testTerrainTexture = TextureAtlas("green01.png");
	initBullet();
	initShader();
	EventManger::GetInstance().GetEvent(EventManger::Events::TEST_BEGIN)();
}

btDefaultCollisionConfiguration *collConf;
btCollisionDispatcher *dispatcher;
btBroadphaseInterface *overlapPairCache;
btSequentialImpulseConstraintSolver *solver;
btDiscreteDynamicsWorld *dynaWorld;

void TestManger::initBullet() {LOGFUNC;
	collConf = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collConf);
	overlapPairCache = new btDbvtBroadphase(); // btAxis3Sweep
	solver = new btSequentialImpulseConstraintSolver();

	dynaWorld = new btDiscreteDynamicsWorld(dispatcher, overlapPairCache, solver, collConf);
	dynaWorld->setGravity(btVector3(btScalar(GlobalGravity.x), btScalar(GlobalGravity.y), btScalar(GlobalGravity.z)));

	dynaWorld->setInternalTickCallback(cubeTickCallback);

	btCollisionShape *ground = new btBoxShape(btVector3(btScalar(25 * 64), btScalar(25 * 64), btScalar(0.1)));
	btTransform groundTrans;
	groundTrans.setIdentity();
	groundTrans.setOrigin(btVector3(25*64, 0, 0));
	btDefaultMotionState *grMotionState = new btDefaultMotionState(groundTrans);
	btRigidBody::btRigidBodyConstructionInfo grInfo(0., grMotionState, ground);
	btRigidBody *grBody = new btRigidBody(grInfo);
	grBody->setRestitution(btScalar(1));
	grBody->setContactProcessingThreshold(btScalar(64));
	auto x = new PhysicsObject(true);
	grBody->setUserPointer(x);
	dynaWorld->addRigidBody(grBody);
}

void _readall(ifstream &str, string &dest) {LOGFUNC;
	istreambuf_iterator<char> beg(str), end;
 	dest = string(beg, end);
}

void TestManger::initShader() {LOGFUNC;
	if (Shader::isAvailable()) {
		// extShader = new Shader();
		ifstream is;
		string vert, frag;
		is.open("vert.vert");
		_readall(is, vert);
		is.close();

		is.open("frag.frag");
		_readall(is, frag);
		is.close();

		extShader.loadFromMemory(vert, sf::Shader::Vertex);
		extShader.loadFromMemory(frag, sf::Shader::Fragment);
	}
}

int disToCamera(const Abs_Abstract *src) {//LOGFUNC;
	auto c = src->GetCoord();
	auto d = Session::GetInstance().CameraLocation;
	return (pow(d.x*10-c.x, 2) + pow(d.y*10-c.y, 2) + pow(d.z*10-c.z, 2));
}

void RenderLayer::sort_Objects() {LOGFUNC;
	sort(Objects.begin(), Objects.end(),
			[](const Abs_Abstract *a, const Abs_Abstract *b) -> bool {
				return disToCamera(a) < disToCamera(b);
			});
}