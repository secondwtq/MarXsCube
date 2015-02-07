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

void ObjectManger::FinishRemove() { LOGFUNC;
	for (auto i : ObjectsToDelete) {
		delObject_all(i, Objects);
		delete i;
	}
	ObjectsToDelete.clear();
}

void TestManger::initTest() {LOGFUNC;
	testTerrainTexture = TextureAtlas("green01.png");
	
	Generic::PhysicsGeneral()->init();
	initShader();
	EventManger::GetInstance().GetEvent(EventManger::Events::TEST_BEGIN)();
}

void _readall(ifstream &str, string &dest) {LOGFUNC;
	istreambuf_iterator<char> beg(str), end;
 	dest = string(beg, end);
}

void TestManger::initShader() {LOGFUNC;
	if (Shader::isAvailable()) {
		ifstream is;
		string vert, frag;
		is.open("./Shaders/vert.vert");
		_readall(is, vert);
		is.close();

		is.open("./Shaders/frag.frag");
		_readall(is, frag);
		is.close();

		extShader.loadFromMemory(vert, sf::Shader::Vertex);
		extShader.loadFromMemory(frag, sf::Shader::Fragment);
	}
}

int disToCamera(const Abs_Abstract *src) {//LOGFUNC;
	if (src->temp_ZOffset) return 0x6FFFFFFF;
	auto c = src->GetCoord();
	auto d = Generic::Session()->CameraLocation;
	int f = d.x*10-c.x, g = d.y*10-c.y, h = d.z*10-c.z;
	return (f*f+g*g+h*h);
}

void RenderLayer::sort_Objects() {
	sort(Objects.begin(), Objects.end(),
			[](const Abs_Abstract *a, const Abs_Abstract *b) -> bool {
				return disToCamera(a) > disToCamera(b); });
}