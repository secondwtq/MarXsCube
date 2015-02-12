#ifndef CUBE_E_OBJMANGER_H
#define CUBE_E_OBJMANGER_H

#include <vector>
#include "Common.h"
#include "SFML.h"
#include <unordered_map>
#include "ObjectArray.h"
using namespace std;
using namespace sf;

class Abs_Abstract;

class ObjectManger {
	public:
		static ObjectManger *instance;
		static inline ObjectManger &GetInstance() { return *instance; }

		void addObject(Abs_Abstract &src) {LOGFUNC; Objects.push_back(&src); }

		vector<Abs_Abstract *> Objects;
		vector<Abs_Abstract *> ObjectsToDelete;

		void RemoveObject(Abs_Abstract *src) {LOGFUNC; ObjectsToDelete.push_back(src); }
		void FinishRemove();
};

class RenderLayer {
	public:
		vector<Abs_Abstract *> Objects;
		void Update();

		bool addObject(Abs_Abstract &src) {LOGFUNC; return addObject_noDup(&src, Objects); }

		void RemoveObject(Abs_Abstract *src) {LOGFUNC; delObject_all(src, Objects); }
};

class CRenderLayerManger {
	public:
		RenderLayer Layers[RenderLayerType::Count];

		void RemoveObject(Abs_Abstract *src);
};

class TextureManger {
	public:
		static TextureManger *instance;
		static inline TextureManger &GetInstance() {LOGFUNC; return *instance; }

		unordered_map<string, TextureAtlas *> TextureHashs;

		int t(int x);
		inline TextureAtlas *LUA_getTexture(string name) {LOGFUNC; return getTexture(name); }
		TextureAtlas *getTexture(const string &name);
};

class TestManger {
	public:
		static TestManger *instance;
		static inline TestManger &GetInstance() { return *instance; }

		TextureAtlas testTerrainTexture;
		RenderWindow *window = nullptr;
		sf::Shader extShader;

		void initTest();

		void initShader();

		TestManger() {LOGFUNC; }
};

#include "Abstract.h"

#endif