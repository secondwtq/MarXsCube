#include "ObjectManger.h"
#include "Abstract.h"
#include "SFML.h"
#include "Generic.h"

#include "Session.h"

//Session *Session::instance = new Session();
const CoordStruct Session::CameraLocationDefault = CoordStruct(384, 384, 320);

unsigned int Session::addObject(Abs_Abstract &src) {LOGFUNC;
	src.RTTIID = ++MaxID;
	return MaxID;
}

void Session::removeObject(Abs_Abstract *src) {LOGFUNC;
	Generic::RenderLayerManger()->RemoveObject(src);
	ObjectManger::GetInstance().RemoveObject(src);
}

void Session::CameraMove(DirectionType::Type Direction, float offest) {LOGFUNC;
	switch (Direction) {
		case DirectionType::Type::SouthWest:
		    this->CameraOffestX -= offest;
		    this->CameraOffestY += offest;
		    break;
		case DirectionType::Type::NorthEast:
		    this->CameraOffestX += offest;
		    this->CameraOffestY -= offest;
		  	break;
		case DirectionType::Type::NorthWest:
		    this->CameraOffestX -= offest;
		    this->CameraOffestY -= offest;
		  	break;
		case DirectionType::Type::SouthEast:
		    this->CameraOffestX += offest;
		    this->CameraOffestY += offest;
		  	break;
		case DirectionType::Type::East:
			this->CameraOffestX += offest;
			break;
		case DirectionType::Type::West:
			this->CameraOffestX -= offest;
			break;
		case DirectionType::Type::North:
			this->CameraOffestY -= offest;
			break;
		case DirectionType::Type::South:
			this->CameraOffestY += offest;
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
