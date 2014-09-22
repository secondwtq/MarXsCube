#include "Common.h"
#include "Techno.h"
#include "SFML.h"
#include "Transform.h"
#include "ObjectManger.h"

using namespace sf;

void RenderElement_DirectionedStatic::Render(CoordStruct &&loc) {LOGFUNC;
	// printf("CubeCore: RenderElement_DirectionedStatic::Render - Rendering ...\n");
	if (Enabled) {
		texture->CenterPivot(renderSprite);
		if (!UseShadowProjection)
			renderSprite.setPosition(GetViewPos(loc+offset));
		else {
			auto _loc = loc + offset;
			_loc = CoordStruct(ProjectionVector.z*_loc.x+ProjectionVector.x*_loc.z, ProjectionVector.z*_loc.y+ProjectionVector.y*_loc.z, 0);
			// cout << _loc.x << " " << _loc.y << endl;
			renderSprite.setPosition(GetViewPos(_loc));
		}
		renderSprite.setTexture(*texture);
		texture->setArea(renderSprite, getCurrentFrame());
		InternalDraw_Ext(*this, renderSprite);
	}
}

void RenderElement_FramedStatic::Render(CoordStruct &&loc) {LOGFUNC;
	if (Enabled) {
		texture->CenterPivot(renderSprite);
		if (!UseShadowProjection)
			renderSprite.setPosition(GetViewPos(loc+offset));
		else {
			auto _loc = loc + offset;
			_loc = CoordStruct(ProjectionVector.z*_loc.x+ProjectionVector.x*_loc.z, ProjectionVector.z*_loc.y+ProjectionVector.y*_loc.z, 0);
			renderSprite.setPosition(GetViewPos(_loc));
		}
		renderSprite.setTexture(*texture);
		texture->setArea(renderSprite, currentFrame);
		InternalDraw_Ext(*this, renderSprite);
	}
}

void RenderElementsContainer::Update() {LOGFUNC;
	// printf("%d %d %d\n", parent->GetCoord().x, parent->GetCoord().y, parent->GetCoord().z);
	// printf("CubeCore: RenderElementsContainer::Update - Updating ...\n");
	for (auto e : elements) e.second->Render(parent->GetCoord());
}

void RenderElementsContainer::setDirection(int degree) {LOGFUNC;
	for (auto e : elements) e.second->direction = degree;
}

Abs_Techno::Abs_Techno(Type_Techno *Type) : Abs_Object(Type), TechnoType(Type), elements(new RenderElementsContainer(this)) {LOGFUNC;
	cout << "CubeCore: Abs_Techno::Abs_Techno - Constructing ..." << endl;
	Layer = Abs_Techno::GetDefaultLayer();
	EventManger::GetInstance().GetEvent(EventManger::Events::TECHNO_CREATE)(this, ExtTable);
	// EnablePhysics = true;
}

void Abs_Techno::Update() {LOGFUNC;
	EventManger::GetInstance().GetEvent(EventManger::Events::TECHNO_UPDATE)(this, ExtTable);
}

void Abs_Techno::Render() {LOGFUNC;
	elements->Update();
}

void Abs_Techno::SpawnAtMapCoord(const CoordStruct &location) {LOGFUNC;
	cout << "CubeCore: Abs_Techno::SpawnAtMapCoord - Spawning " << location.x << " " << location.y << " " << location.z << " ..." << endl;
    this->setLocation(location);
	ObjectManger::GetInstance().addObject(*this);
	RenderLayerManger::GetInstance().Layers[Layer].addObject(*this);
	Physics->SpawnAt(location);
	EventManger::GetInstance().GetEvent(EventManger::Events::TECHNO_SPAWN)(this, ExtTable);
}

Abs_Techno::~Abs_Techno() { delete elements; }