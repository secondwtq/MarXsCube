#include "Abstract.h"
#include "ObjectArray.h"
#include "ObjectTypeArray.h"

#include "Session.h"
#include "Config.h"
#include "Generic.h"

Abs_Abstract::Abs_Abstract() : Abs_Abstract(new PhysicsObject(this)) { }

Abs_Abstract::Abs_Abstract(PhysicsObject *phy_object) :
	RTTIID(Generic::Session()->addObject(*this)),
	Physics(phy_object),
	ExtTable(EventManger::GetInstance().CreateObjectTable(*this)) { }

Abs_Abstract::~Abs_Abstract() {LOGFUNC;
	std::cout << "CubeCore: Abs_Abstract::~Abs_Abstract - Destroying ObjectTable " << this->RTTIID << " ..." << std::endl;
	EventManger::GetInstance().DestroyObjectTable(*this);

	delete Physics;
}

void Abs_Abstract::setLayer(int Layer) { this->Layer = (RenderLayerType::Type)Layer; }
