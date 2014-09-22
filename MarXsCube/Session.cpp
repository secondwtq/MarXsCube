#include "Session.h"
#include "ObjectManger.h"
#include "Abstract.h"
#include "SFML.h"

Session *Session::instance = new Session();
const CoordStruct Session::CameraLocationDefault = CoordStruct(384, 384, 320);

void Session::initSession() {LOGFUNC;
	CameraObject = new Abs_CameraObject();

	ObjectManger::GetInstance().addObject(*CameraObject);
	RenderLayerManger::GetInstance().Layers[RenderLayerType::Type::LAYER_UI].addObject(*CameraObject);
}

unsigned int Session::addObject(Abs_Abstract &src) {LOGFUNC;
	src.RTTIID = ++MaxID;
	return MaxID;
}

void Session::removeObject(Abs_Abstract *src) {LOGFUNC;
	RenderLayerManger::GetInstance().RemoveObject(src);
	ObjectManger::GetInstance().RemoveObject(src);
	// delete src;
}

void Session::CameraMove(DirectionType::Type Direction, float offest) {LOGFUNC;
	switch (Direction) {
		case DirectionType::Type::SouthWest:
		    Session::GetInstance().CameraOffestX -= offest;
		    Session::GetInstance().CameraOffestY += offest;
		    break;
		case DirectionType::Type::NorthEast:
		    Session::GetInstance().CameraOffestX += offest;
		    Session::GetInstance().CameraOffestY -= offest;
		  	break;
		case DirectionType::Type::NorthWest:
		    Session::GetInstance().CameraOffestX -= offest;
		    Session::GetInstance().CameraOffestY -= offest;
		  	break;
		case DirectionType::Type::SouthEast:
		    Session::GetInstance().CameraOffestX += offest;
		    Session::GetInstance().CameraOffestY += offest;
		  	break;
		case DirectionType::Type::East:
			Session::GetInstance().CameraOffestX += offest;
			break;
		case DirectionType::Type::West:
			Session::GetInstance().CameraOffestX -= offest;
			break;
		case DirectionType::Type::North:
			Session::GetInstance().CameraOffestY -= offest;
			break;
		case DirectionType::Type::South:
			Session::GetInstance().CameraOffestY += offest;
			break;
	}
	UpdateCameraOffest();
}

void Session::setKeyEvent(sf::Event::KeyEvent s) {LOGFUNC;
	KeyData.code = s.code;
	KeyData.alt = s.alt, KeyData.control = s.control, KeyData.shift = s.shift, KeyData.system = s.system;
}

void Session::setMousePos_Ab() {LOGFUNC;
	auto p = sf::Mouse::getPosition();
	MousePosData.x_absolute = p.x, MousePosData.y_absolute = p.y;
}

void Session::updateMouseButtonStatus() {LOGFUNC;
	MousePosData.left = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
	MousePosData.right = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
	MousePosData.middle = sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle);
}

void Session::setMousePos_Rl(sf::Event::MouseButtonEvent &e) {LOGFUNC;
	MousePosData.pos.x = e.x, MousePosData.pos.y = e.y;
}

void Session::setMousePos_Rl(sf::Event::MouseMoveEvent &e) {LOGFUNC;
	MousePosData.pos.x = e.x, MousePosData.pos.y = e.y;
}

void Session::setMousePos_Release(sf::Event::MouseButtonEvent &e) {LOGFUNC;
	setMousePos_Rl(e);
	switch (e.button) {
		case sf::Mouse::Button::Left: MousePosData.left = true; break;
		case sf::Mouse::Button::Right: MousePosData.right = true; break;
		case sf::Mouse::Button::Middle: MousePosData.middle = true; break;
		default: break;
	}
}

void Session::setMousePos_Press(sf::Event::MouseButtonEvent &e) {LOGFUNC;
	setMousePos_Rl(e);
}

void Session::LUA_CameraMove(unsigned short Direction, float offest) {LOGFUNC; CameraMove((DirectionType::Type)Direction, offest); }