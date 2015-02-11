#include "Common.h"
#include "Techno.h"
#include "SFML.h"
#include "Transform.h"
#include "ObjectManger.h"
#include "RenderElementsContainer.h"
#include "Generic.h"

using namespace sf;

Abs_Techno::Abs_Techno(Type_Techno *Type) : Abs_Object(Type), TechnoType(Type), elements(new RenderElementsContainer(this)) {LOGFUNC;
	cout << "CubeCore: Abs_Techno::Abs_Techno - Constructing ..." << endl;
	Layer = Abs_Techno::GetDefaultLayer();
	EventManger::GetInstance().GetEvent(EventManger::Events::TECHNO_CREATE)(this, ExtTable);
}

Abs_Techno::Abs_Techno(Type_Techno *Type, PhysicsObject *phy_object) :
	Abs_Object(Type, phy_object), TechnoType(Type), elements(new RenderElementsContainer(this)) {
	cout << "CubeCore: Abs_Techno::Abs_Techno - Constructing without Physics ..." << endl;
	Layer = Abs_Techno::GetDefaultLayer();
	EventManger::GetInstance().GetEvent(EventManger::Events::TECHNO_CREATE)(this, ExtTable);
}

void Abs_Techno::Update() {LOGFUNC;
	EventManger::GetInstance().GetEvent(EventManger::Events::TECHNO_UPDATE)(this);
}

void Abs_Techno::Render() {LOGFUNC;
	elements->Update();
}

void Abs_Techno::SpawnAtMapCoord(const CoordStruct &location) {LOGFUNC;
	cout << "CubeCore: Abs_Techno::SpawnAtMapCoord - Spawning " << location.x << " " << location.y << " " << location.z << " ..." << endl;
    this->setLocation(location);
	ObjectManger::GetInstance().addObject(*this);
	Generic::RenderLayerManger()->Layers[Layer].addObject(*this);
	if (this->Physics) Physics->SpawnAt(location);
	EventManger::GetInstance().GetEvent(EventManger::Events::TECHNO_SPAWN)(this, ExtTable);
}

Abs_Techno::~Abs_Techno() { delete elements; }