#include "Anim.h"
#include "ObjectManger.h"
#include "Session.h"

#include "Type_Anim.h"

#include "MotionStateTest.h"

#include "Config.h"

using namespace luabridge;

extern unordered_map<btRigidBody *, Abs_Object *> phyMap;

Abs_Anim::Abs_Anim(Type_Anim *Type) : Abs_Object(Type), AnimType(Type) {LOGFUNC;
	cout << "CubeCore: Abs_Anim::Abs_Anim - Constructing ..." << endl;
	EventManger::GetInstance().GetEvent(EventManger::Events::ANIM_CREATE)(this, ExtTable);
	renderSprite.scale(Type->_scale);
}

void Abs_Anim::Update() {LOGFUNC;
	EventManger::GetInstance().GetEvent(EventManger::Events::ANIM_UPDATE)(this, ExtTable);
	if (AnimTimer.TimerEnded)
		Session::GetInstance().removeObject(this);
}

void Abs_Anim::Render() {LOGFUNC;
	if (AnimTimer.Enabled) {
		AnimType->texture->CenterPivot(renderSprite);
		renderSprite.setPosition(GetViewPos(GetCoord())+AnimType->_offset);
		AnimType->texture->setArea_(renderSprite, GetCurrentFrame());
		TestManger::GetInstance().window->draw(renderSprite);
	}
	if (AnimTimer.Enabled)
		AnimTimer.Update();
}

void Abs_Anim::StartPlay() {LOGFUNC;
	AnimTimer.Reset((AnimType->texture->totalTexNum * AnimType->FrameLast)/AnimType->FrameStep, AnimType->LoopCount);
	AnimTimer.SwitchStart();
}

void Abs_Anim::SpawnAtMapCoord(const CoordStruct &location) {LOGFUNC;
	cout << "CubeCore: Abs_Anim::SpawnAtMapCoord - Spawning " << location.x << " " << location.y << " " << location.z << " ..." << endl;
	this->setLocation(location);
	ObjectManger::GetInstance().addObject(*this);
	RenderLayerManger::GetInstance().Layers[Layer].addObject(*this);
	cout << "CubeCore: Abs_Anim::SpawnAtMapCoord - Starting ..." << endl;
	StartPlay();
	EventManger::GetInstance().GetEvent(EventManger::Events::ANIM_SPAWN)(this, ExtTable);
	Physics->SpawnAt(location);
}

unsigned int Abs_Anim::GetCurrentFrame() {LOGFUNC;
	// cout << "CubeCore: Abs_Anim::GetCurrentFrame - Returning " << ((AnimTimer.GetCurrent() / AnimType->FrameLast) * AnimType->FrameStep) << " ..." << endl;
	return (AnimTimer.GetCurrent() / AnimType->FrameLast) * AnimType->FrameStep;
}