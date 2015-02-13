#include "Map.h"

#include "Type_Anim.h"
#include "Anim.h"

#include "Config.h"
#include "Session.h"
#include "Physics.h"

#include "Generic.h"

#include "ObjectManger.h"

#include <fstream>

ObjectManger *ObjectManger::instance = new ObjectManger();
TestManger *TestManger::instance = new TestManger();
TextureManger *TextureManger::instance = new TextureManger();

TextureAtlas *TextureManger::getTexture(const string &name) {LOGFUNC; return TextureHashs[name]; }

void RenderLayer::Update() {LOGFUNC;
	for (size_t i = 0; i < Objects.size(); i++)
		Objects[i]->Render();
}

void CRenderLayerManger::RemoveObject(Abs_Abstract *src) {LOGFUNC;
	// for(auto l : Layers) {
	// 	printf("removing ...\n");
	// 	l.RemoveObject(src);
	// }
	for(size_t i = 0; i < RenderLayerType::Count; i++)
		Layers[i].RemoveObject(src);
}

void ObjectManger::FinishRemove() { LOGFUNC;
	for (auto i : ObjectsToDelete) {
		delObject_all(i, Objects);
		delete i;
	}
	ObjectsToDelete.clear();
}

void TestManger::initTest() {LOGFUNC;
	Generic::PhysicsGeneral()->init();
	EventManger::GetInstance().GetEvent(EventManger::Events::TEST_BEGIN)();
}

void _readall(ifstream &str, string &dest) {LOGFUNC;
	istreambuf_iterator<char> beg(str), end;
 	dest = string(beg, end);
}

int disToCamera(const Abs_Abstract *src) {
//	if (src->temp_ZOffset) return 0x6FFFFFFF;
	auto c = src->GetCoord();
	static auto d = Generic::Session()->CameraLocation;
	int f = d.x*10-c.x, g = d.y*10-c.y, h = d.z*10-c.z;
	return (f*f+g*g+h*h);
}