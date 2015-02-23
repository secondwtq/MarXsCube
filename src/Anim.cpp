#include "Anim.h"
#include "ObjectManger.h"
#include "Session.h"

#include "Type_Anim.h"

#include "MotionStateTest.h"

#include "Generic.h"

#include "Config.h"

using namespace luabridge;

Abs_Anim::Abs_Anim(Type_Anim *Type) : Abs_Object(Type, nullptr), AnimType(Type) {LOGFUNC;
	cout << "CubeCore: Abs_Anim::Abs_Anim - Constructing ..." << endl;
	EventManger::GetInstance().GetEvent(EventManger::Events::ANIM_CREATE)(this, ExtTable);
//	renderSprite.scale(Type->_scale);
}

void Abs_Anim::Update() {
	EventManger::GetInstance().GetEvent(EventManger::Events::ANIM_UPDATE)(this, ExtTable);
	if (AnimTimer.TimerEnded)
		Generic::RemoveObject(this);
}

void Abs_Anim::Render() {
	if (AnimTimer.Enabled) {
		AnimTimer.Update();
		
		if (!AnimTimer.TimerEnded) {
			sprite.position = CubeTransform::view_pos_nt(GetCoord()+AnimType->Offset);
			sf::IntRect tex_area = AnimType->texture->getArea(this->GetCurrentFrame());
			sprite.set_texture_area(tex_area.left, tex_area.top, tex_area.width*scale, tex_area.height*scale);
		}
	}
}

void Abs_Anim::StartPlay() {
	
	Silcon::Manger->register_sprite(&this->sprite);
	this->sprite.set_texture(*AnimType->texture);
	
	AnimTimer.Reset((AnimType->texture->totalTexNum * AnimType->FrameLast)/AnimType->FrameStep, AnimType->LoopCount);
	AnimTimer.SwitchStart();
}

void Abs_Anim::SpawnAtMapCoord(const CoordStruct &location) {LOGFUNC;
	cout << "CubeCore: Abs_Anim::SpawnAtMapCoord - Spawning " << location.x << " " << location.y << " " << location.z << " ..." << endl;
	this->setLocation(location);
	ObjectManger::GetInstance().addObject(*this);
	Generic::RenderLayerManger()->Layers[Layer].addObject(*this);
	StartPlay();
	EventManger::GetInstance().GetEvent(EventManger::Events::ANIM_SPAWN)(this, ExtTable);
}

unsigned int Abs_Anim::GetCurrentFrame() {
	return (AnimTimer.GetCurrent() / AnimType->FrameLast) * AnimType->FrameStep; }