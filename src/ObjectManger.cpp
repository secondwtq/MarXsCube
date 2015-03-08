#include "Type_Anim.h"
#include "Anim.h"

#include "Config.h"
#include "Session.h"
#include "Physics.h"

#include "Generic.h"

#include "ObjectManger.h"

#include <fstream>

ObjectManger *ObjectManger::instance = new ObjectManger();
TextureManger *TextureManger::instance = new TextureManger();

TextureAtlas *TextureManger::getTexture(const string &name) {LOGFUNC; return TextureHashs[name]; }

void RenderLayer::Update() {LOGFUNC;
	for (size_t i = 0; i < Objects.size(); i++)
		Objects[i]->Render();
}

void CRenderLayerManger::RemoveObject(Abs_Abstract *src) {LOGFUNC;
	for(size_t i = 0; i < RenderLayerType::Count; i++)
		Layers[i].RemoveObject(src); }

void ObjectManger::FinishRemove() { LOGFUNC;
	for (auto i : ObjectsToDelete) {
		delObject_all(i, Objects);
		delete i;
	}
	ObjectsToDelete.clear();
}
