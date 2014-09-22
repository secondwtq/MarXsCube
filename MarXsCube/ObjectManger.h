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
		void FinishRemove() { LOGFUNC;
			for (auto i : ObjectsToDelete) {
				delObject_all(i, Objects);
				delete i;
			}
			ObjectsToDelete.clear();
		}
};

class RenderLayer {
	public:
		vector<Abs_Abstract *> Objects;
		void sort_Objects();
		void Update();

		bool addObject(Abs_Abstract &src) {LOGFUNC; return addObject_noDup(&src, Objects); }

		void RemoveObject(Abs_Abstract *src) {LOGFUNC; delObject_all(src, Objects); }
};

class RenderLayerManger {
	public:
		static RenderLayerManger *instance;
		static inline RenderLayerManger &GetInstance() { return *instance; }

		RenderLayer Layers[RenderLayerType::Count];

		void RemoveObject(Abs_Abstract *src);
};

class TextureManger {
	public:
		static TextureManger *instance;
		static inline TextureManger &GetInstance() {LOGFUNC; return *instance; }

		unordered_map<string, TextureAtlas *> TextureHashs;

		int t(int x);
		TextureAtlas *LUA_getTexture(string name) {LOGFUNC; return getTexture(name); }
		TextureAtlas *getTexture(const string &name);
		TextureAtlas &getTexture_(string &name);
};

class TestManger {
	public:
		static TestManger *instance;
		static inline TestManger &GetInstance() { return *instance; }

		TextureAtlas testTerrainTexture;
		RenderWindow *window = nullptr;
		sf::Shader extShader;

		void initTest();

		void initBullet();

		void initShader();

		TestManger() {LOGFUNC; }
};

#include "Abstract.h"

#endif