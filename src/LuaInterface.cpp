#include "Common.h"
#include "LuaInterface.h"

#include "Abstract.h"
#include "Object.h"
#include "Anim.h"
#include "Cell.h"

#include "Type_Abstract.h"
#include "Type_Object.h"
#include "Type_Anim.h"

#include "Map.h"
#include "Session.h"

#include "SFML.h"
#include "LuaUtils.h"
#include "Pathfinder.h"

#include "Techno.h"
#include "Type_Techno.h"

#include "RenderElement.h"
#include "RenderElementsContainer.h"

#include "Generic.h"

#include "StdInterface.h"
#include "GLM_Interface.h"
#include "RayTest_Interface.h"
#include "BulletVehicle_Interface.h"
#include "Tesla_Interface.h"
#include "Grit_Interface.h"
#include "Acheron_Interface.h"
#include "Wonderland_Interface.h"
#include "BulletInterface.h"
#include "Path_Interface.h"
#include "Serialization_Interface.h"

#include "Gmap_Interface.h"

#include "Timer.h"

using namespace luabridge;

namespace LuaInterface {

	void RegisterInterface(LuaStatus &L) {
		RegisterInterface_Timer(L);
		RegisterInterface_Util(L);
		RegisterInterface_Abstract(L);
		RegisterInterface_Physics(L);
		RegisterInterface_Cell(L);
		RegisterInterface_Object(L);
		RegisterInterface_Anim(L);
		RegisterInterface_Techno(L);
		RegisterInterface_Map(L);
		RegisterInterface_New(L);
		
		RegisterInterface_GLM(L);
		RegisterInterface_User_Gmap(L);
		
		RegisterInterface_Tesla(L);
		RegisterInterface_Grit(L);
		RegisterInterface_Acheron(L);
		RegisterInterface_Wonderland(L);
		RegisterInterface_Bullet(L);
		RegisterInterface_Path(L);
		RegisterInterface_Serialization(L);
		
		RegisterInterface_Std<int>(L, "int");
		RegisterInterface_Std<CubePoint>(L, "CubePoint");
	}
	
	void RegisterInterface_New(LuaStatus &L) {
		getGlobalNamespace(L).
			beginNamespace("CoreHelpers").
				addFunction("texture", &LuaUtils::Helper_GetTexture).
				addFunction("toTechno", &LuaUtils::toTechno).
			endNamespace();
	}
	
	void RegisterInterface_Util(LuaStatus &L) {
		getGlobalNamespace(L).
			addFunction("YouFuckingNULLPOINTER", &LuaUtils::CreateObjectTable_Default).
			beginNamespace("Utility").
				addFunction("closeSession", &LuaUtils::closeSession).
				addFunction("GetCoordFromPoint", &LuaUtils::GetCoordFromPoint).
				addFunction("Pathfinding_Find", &Pathfinding::Find).
				addFunction("GetTime", &LuaUtils::GetTime).
				addFunction("GetTime_Clock", &LuaUtils::GetTime_Clock).
				addFunction("time_in_ms", &time_in_millisec).
				addFunction("time_in_millisec", &time_in_millisec).
				addFunction("toTechno", &LuaUtils::toTechno).
				addFunction("torad", &torad).
				beginNamespace("Pathfinding").
					addFunction("smooth_slow", &Pathfinding::smooth_slow).
				endNamespace().
				beginClass<Pathfinding::PathFindingcache>("PathFindingcache").
					addFunction("reset", &Pathfinding::PathFindingcache::reset).
					addFunction("inc", &Pathfinding::PathFindingcache::inc).
					addFunction("getCur", &Pathfinding::PathFindingcache::getCur).
					addFunction("ended", &Pathfinding::PathFindingcache::ended).
					addFunction("inside", &Pathfinding::PathFindingcache::inside).
					addFunction("jumpTo", &Pathfinding::PathFindingcache::jumpTo).
					addFunction("getNex", &Pathfinding::PathFindingcache::getNex).
					addFunction("getNexNex", &Pathfinding::PathFindingcache::getNexNex).
				endClass().
				beginClass<Pathfinding::Repathcache>("Repathcache").
					addFunction("inside", &Pathfinding::Repathcache::inside).
					addFunction("insert", &Pathfinding::Repathcache::insert).
					addFunction("clear", &Pathfinding::Repathcache::clear).
					addFunction("size", &Pathfinding::Repathcache::size).
					addFunction("resize", &Pathfinding::Repathcache::resize).
				endClass().
				beginClass<ObjectCollideCache>("ObjectCollideCache").
					addFunction("isempty", &ObjectCollideCache::isempty).
					addFunction("insert", &ObjectCollideCache::insert).
					addFunction("clear", &ObjectCollideCache::clear).
				endClass().
			endNamespace();
	}

	void RegisterInterface_Timer(LuaStatus &L) {
		getGlobalNamespace(L).
			beginNamespace("Utility").
				beginClass<CubeTimer>("CubeTimer").
					addConstructor<void (*)(int, int)>().
					addData("Enabled", &CubeTimer::Enabled).
					addData("Paused", &CubeTimer::Paused).
					addData("TimerEnded", &CubeTimer::TimerEnded).
					addData("current", &CubeTimer::current).
					addData("Interval", &CubeTimer::Interval).
					addData("LoopCount", &CubeTimer::LoopCount).
					addData("LoopTimed", &CubeTimer::LoopTimed).
					addFunction("Update", &CubeTimer::Update).
					addFunction("ResetToEnd", &CubeTimer::ResetToEnd).
					addFunction("SwitchStart", &CubeTimer::SwitchStart).
					addFunction("GetCurrent", &CubeTimer::GetCurrent_LUA).
					addFunction("GetPercentage", &CubeTimer::GetPercentage_LUA).
					addStaticFunction("createCubeTimer", &CubeTimer::createCubeTimer).
				endClass().
				beginClass<CoordStruct>("CoordStruct").
					addData("x", &CoordStruct::x).
					addData("y", &CoordStruct::y).
					addData("z", &CoordStruct::z).
					addConstructor<void (*)(int, int, int)>().
				endClass().
				beginClass<CellStruct>("CellStruct").
					addData("x", &CellStruct::x).
					addData("y", &CellStruct::y).
					addConstructor<void (*)(int, int)>().
				endClass().
				beginClass<CubePoint>("CubePoint").
					addData("x", &CubePoint::x).
					addData("y", &CubePoint::y).
					addConstructor<void (*)(int, int)>().
				endClass().
				beginClass<Homogeneous4D>("Homogeneous4D").
					addData("x", &Homogeneous4D::x).
					addData("y", &Homogeneous4D::y).
					addData("z", &Homogeneous4D::z).
					addData("w", &Homogeneous4D::w).
					addConstructor<void (*)(float, float, float, float)>().
				endClass().
				beginClass<Float3D>("Float3D").
					addData("x", &Float3D::x).
					addData("y", &Float3D::y).
					addData("z", &Float3D::z).
					addConstructor<void (*)(float, float, float)>().
				endClass().
				beginClass<sf::Vector2f>("SF_Vector2f").
					addData("x", &sf::Vector2f::x).
					addData("y", &sf::Vector2f::y).
					addConstructor<void (*)(float, float)>().
				endClass().
				beginClass<CubeKeyEvent>("KeyEvent").
					addData("code", &CubeKeyEvent::code).
					addData("control", &CubeKeyEvent::control).
					addData("shift", &CubeKeyEvent::shift).
					addData("alt", &CubeKeyEvent::alt).
					addData("system", &CubeKeyEvent::system).
				endClass().
				beginClass<CubeMousePos>("MousePosition").
					addData("pos", &CubeMousePos::pos).
					addData("x_absolute", &CubeMousePos::x_absolute).addData("y_absolute", &CubeMousePos::y_absolute).
					addData("left", &CubeMousePos::left).addData("right", &CubeMousePos::right).addData("middle", &CubeMousePos::middle).
				endClass().
				beginClass<TextureAtlas>("TextureAtlas").
					addData("numTextures", &TextureAtlas::TexNum).
				endClass().
				beginClass<RenderElement>("RenderElement").
					addData("Enabled", &RenderElement::Enabled).
					addData("offset", &RenderElement::offset).
					addData("colorMultiply", &RenderElement::colorMultiply).
					addData("direction", &RenderElement::direction).
					addData("direction_offset", &RenderElement::direction_offset).
					addData("UseShadowProjection", &RenderElement::UseShadowProjection).
					addData("ProjectionVector", &RenderElement::ProjectionVector).
					addData("z_index", &RenderElement::z_index).
					addData("parent", &RenderElement::parent).
				endClass().
				deriveClass<RenderElement_DirectionedStatic, RenderElement>("RenderElement_DirectionedStatic").
					addConstructor<void (*)(TextureAtlas *, int)>().
					addData("frameCount", &RenderElement_DirectionedStatic::frameCount).
					addStaticFunction("createElement", &RenderElement_DirectionedStatic::createElement).
					addStaticFunction("create", &RenderElement_DirectionedStatic::createElement).
				endClass().
				deriveClass<RenderElement_FramedStatic, RenderElement>("RenderElement_FramedStatic").
					addConstructor<void (*)(TextureAtlas *)>().
					addData("currentFrame", &RenderElement_FramedStatic::currentFrame).
					addFunction("setCurrentFrame", &RenderElement_FramedStatic::setCurrentFrame).
					addStaticFunction("createElement", &RenderElement_FramedStatic::createElement).
					addStaticFunction("create", &RenderElement_FramedStatic::createElement).
				endClass().
				deriveClass<RenderElement_FramedDynamic, RenderElement>("RenderElement_FramedDynamic").
					addConstructor<void(*)(TextureAtlas *, USIZE)>().
					addStaticFunction("create", &RenderElement_FramedDynamic::create).
				endClass().
				deriveClass<RenderElement_InternalLine, RenderElement>("RenderElement_InternalLine").
					addConstructor<void (*)(const CoordStruct&, const CoordStruct&, const Vector4DT<float>&)>().
					addData("point1", &RenderElement_InternalLine::point1).
					addData("point2", &RenderElement_InternalLine::point2).
					addData("color", &RenderElement_InternalLine::color).
					addData("thickness", &RenderElement_InternalLine::thickness).
					addStaticFunction("create", &RenderElement_InternalLine::create).
				endClass().
				beginClass<RenderElementsContainer>("RenderElementsContainer").
					addFunction("insert", &RenderElementsContainer::insert).
					addData("direction", &RenderElementsContainer::direction).
				endClass().
				beginClass<ObjectDirection>("ObjectDirection").
					addData("degree", &ObjectDirection::degree).
				endClass().
			endNamespace();
	}

	void RegisterInterface_Abstract(LuaStatus &L) {
		getGlobalNamespace(L).
			beginNamespace("Objects").
				beginClass<Abs_Abstract>("Abs_Abstract").
					addData("RTTIID", &Abs_Abstract::RTTIID).
					addFunction("WhatAmI", &Abs_Abstract::WhatAmI).
					addFunction("GetCoord", &Abs_Abstract::GetCoord).
					addFunction("setLayer", &Abs_Abstract::setLayer).
					addData("EnablePhysics", &Abs_Abstract::EnablePhysics).
					addData("ExtTable", &Abs_Abstract::ExtTable).
					addData("Physics", &Abs_Abstract::Physics).
					addFunction("setLocation", &Abs_Abstract::setLocation).
					addFunction("getMainRotation", &Abs_Abstract::getMainRotation).
					addFunction("onGround", &Abs_Abstract::onGround).
					addFunction("print_this", &Abs_Abstract::print_this).
					addData("useCollSphere", &Abs_Abstract::useCollSphere).
					addData("rCollSphere", &Abs_Abstract::rCollSphere).
				endClass().
				deriveClass<Type_Abstract, Abs_Abstract>("Type_Abstract").
					addData("RegName", &Type_Abstract::RegName).
					addData("ScriptType", &Type_Abstract::typeRef).
					addData("t", &Type_Abstract::t).
				endClass().
			endNamespace();
	}

	void RegisterInterface_Cell(LuaStatus &L) {
		getGlobalNamespace(L).
			beginNamespace("Objects").
				deriveClass<Abs_Cell, Abs_Abstract>("Abs_Cell").
					addData("LocCell", &Abs_Cell::LocCell).
					addData("height", &Abs_Cell::height).
					addData("ShowCenter", &Abs_Cell::ShowCenter).
					addFunction("PassableTo", &Abs_Cell::PassableTo).
					addFunction("GetHeight", &Abs_Cell::GetHeight).
					addData("occupiedByObject", &Abs_Cell::occupiedByObject).
					addFunction("clearOccupyObject", &Abs_Cell::clearOccupyObject).
					addFunction("setOccupyObject", &Abs_Cell::setOccupyObject).
					addFunction("getOccupyObject", &Abs_Cell::getOccupyObject).
				endClass().
			endNamespace();
	}

	void RegisterInterface_Object(LuaStatus &L) {
		getGlobalNamespace(L).
			beginNamespace("Objects").
				deriveClass<Abs_Object, Abs_Abstract>("Abs_Object").
					//addData("Location", &Abs_Object::Location).
					addFunction("SpawnAtMapCoord", &Abs_Object::SpawnAtMapCoord).
					addData("Direction", &Abs_Object::Direction).
					addFunction("getDirection", &Abs_Object::getDirection).
					addData("Destnation", &Abs_Object::Destnation).
					addFunction("setDestnation", &Abs_Object::setDestnation).
					addData("repathcache", &Abs_Techno::repathcache).
					addFunction("getRepathcache", &Abs_Techno::getRepathcache).
				endClass().
				deriveClass<Type_Object, Type_Abstract>("Type_Object").

				endClass().
			endNamespace();
	}

	void RegisterInterface_Anim(LuaStatus &L) {
		getGlobalNamespace(L).
			beginNamespace("Objects").
				deriveClass<Abs_Anim, Abs_Object>("Abs_Anim").
					addData("AnimTimer", &Abs_Anim::AnimTimer).
					addData("AnimType", &Abs_Anim::AnimType).
					addFunction("getAnimTimer", &Abs_Anim::getAnimTimer).
					addFunction("getAnimType", &Abs_Anim::getAnimType).
					addFunction("getObjectPtr", &Abs_Anim::getObjectPtr).
				endClass().
				deriveClass<Type_Anim, Type_Object>("Type_Anim").
					addData("LoopCount", &Type_Anim::LoopCount).
					addData("_scale", &Type_Anim::_scale).
					addStaticFunction("createAnim", &Type_Anim::createAnim).
				endClass().
			endNamespace();
	}

	void RegisterInterface_Techno(LuaStatus &L) {
		getGlobalNamespace(L).
			beginNamespace("Objects").
				deriveClass<Abs_Techno, Abs_Object>("Abs_Techno").
					addData("TechnoType", &Abs_Techno::TechnoType).
					addData("elements", &Abs_Techno::elements).
					addFunction("getTechnoType", &Abs_Techno::getTechnoType).
					addData("pathfindingcache", &Abs_Techno::pathfindingcache).
					addFunction("getPathfindingcache", &Abs_Techno::getPathfindingcache).
				endClass().
				deriveClass<Type_Techno, Type_Object>("Type_Techno").
					addStaticFunction("createTechno", &Type_Techno::createTechno).
					addStaticFunction("createTechno_nophy", &Type_Techno::createTechno_nophy).
				endClass().
			endNamespace();
	}

	void RegisterInterface_Map(LuaStatus &L) {
		getGlobalNamespace(L).
			beginNamespace("Objects").
				beginClass<Map>("Map").
					addStaticFunction("GetInstance", &Map::GetInstance).
					addStaticData("instance", &Map::instance).
					addFunction("CreateEmptyMap", &Map::CreateEmptyMap).
					addFunction("GetCellAt", &Map::GetCellAt).
					addFunction("GetCellAt_Pos", &Map::GetCellAt_Pos).
					addStaticFunction("CellFromCoord", &Map::CellFromCoord).
				endClass().
				beginClass<CSession>("Session").
					addStaticData("instance", &(Generic::session)).
					addStaticFunction("GetInstance", &Generic::Session).
					addFunction("CameraMove", &CSession::LUA_CameraMove).
					addData("KeyData", &CSession::KeyData).
					addData("MousePosData", &CSession::MousePosData).
					addFunction("removeObject", &CSession::removeObject).
				endClass().
				beginClass<TextureManger>("TextureManger").
					addStaticData("instance", &TextureManger::instance).
					addStaticFunction("GetInstance", &TextureManger::GetInstance).
					addFunction("getTexture__", &TextureManger::getTexture).
					addFunction("getTexture", &TextureManger::LUA_getTexture).
				endClass().
				beginClass<ObjectManger>("ObjectManger").
					addStaticData("instance", &ObjectManger::instance).
					addStaticFunction("GetInstance", &ObjectManger::GetInstance).
					// addFunction("").
				endClass().
			endNamespace();
	}

	void RegisterInterface_Physics(LuaStatus &L) {
		getGlobalNamespace(L).
			beginNamespace("Physics").
				beginClass<PhysicsObject>("PhysicsObject").
					addData("attachedToObject", &PhysicsObject::attachedToObject).
					addData("vehicle", &PhysicsObject::vehicle).
					addData("forward_vec", &PhysicsObject::forward_vec).
					addFunction("setDirection", &PhysicsObject::setDirection).
					addFunction("setDirectionTo", &PhysicsObject::setDirectionTo).
					addFunction("setLocation", &PhysicsObject::setLocation).
					addFunction("disableResponse", &PhysicsObject::disableResponse).
					addFunction("setToStatic", &PhysicsObject::setToStatic).
					addFunction("setToKinematic", &PhysicsObject::setToKinematic).
					addFunction("applyImpulse", &PhysicsObject::applyImpulse).
					addFunction("setLinearVelocity", &PhysicsObject::setLinearVelocity).
					addFunction("getLinearVelocity", &PhysicsObject::getLinearVelocity).
					addFunction("setVelocity", &PhysicsObject::setVelocity).
					addFunction("getMainRotationVelocity", &PhysicsObject::getMainRotationVelocity).
					addFunction("setMainRotationVelocity", &PhysicsObject::setMainRotationVelocity).
					addFunction("getVelocity", &PhysicsObject::getVelocity).
					addFunction("applyForce", &PhysicsObject::applyForce).
					addFunction("applyForce_Central", &PhysicsObject::applyForce_Central).
					addFunction("applyForce_Directional", &PhysicsObject::applyForce_Directional).
					addFunction("applyCentralForce_Directional", &PhysicsObject::applyCentralForce_Directional).
					addFunction("activate", &PhysicsObject::activate).
					addFunction("getVerticalVelocity", &PhysicsObject::getVerticalVelocity).
					addFunction("applyCentralForce_Vertical", &PhysicsObject::applyCentralForce_Vertical).
					addFunction("applyCentralImpulse", &PhysicsObject::applyCentralImpulse).
					addFunction("applyCentralImpulse_Vertical", &PhysicsObject::applyCentralImpulse_Vertical).
					addFunction("clearVerticalVelocity", &PhysicsObject::clearVerticalVelocity).
					addFunction("applyCentralImpulse_Directional", &PhysicsObject::applyCentralImpulse_Directional).
					addFunction("applyTorqueImpulse_Initial", &PhysicsObject::applyTorqueImpulse_Initial).
					addFunction("applyImpulse_Vertical", &PhysicsObject::applyImpulse_Vertical).
					addFunction("getMainRotation", &PhysicsObject::getMainRotation).
					addFunction("checkCollide", &PhysicsObject::checkCollide).
					addFunction("checkCollide_", &PhysicsObject::checkCollide_).
				endClass().
			endNamespace();
		
		LuaInterface::RegisterInterface_Physics_RayTest(L);
		LuaInterface::RegisterInterface_Physics_Vehicle(L);
	}

}