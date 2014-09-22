#include "Abstract.h"
#include "ObjectArray.h"
#include "ObjectTypeArray.h"

#include "Session.h"
#include "Config.h"
#include "Generic.h"

Abs_Abstract::Abs_Abstract() : 
	RTTIID(Generic::Session()->addObject(*this)),
	Physics(new PhysicsObject(this)),
	ExtTable(EventManger::GetInstance().CreateObjectTable(*this)) 
	{LOGFUNC; }

Abs_Abstract::~Abs_Abstract() {LOGFUNC;
	std::cout << "CubeCore: Abs_Abstract::~Abs_Abstract - Destroying ObjectTable " << this->RTTIID << " ..." << std::endl;
	EventManger::GetInstance().DestroyObjectTable(*this);

	delete Physics;
}

void Abs_Abstract::setLayer(int Layer) {LOGFUNC; this->Layer = (RenderLayerType::Type)Layer; }