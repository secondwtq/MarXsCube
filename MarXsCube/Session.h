#ifndef CUBE_E_SESSION_H
#define CUBE_E_SESSION_H

#include "Common.h"
#include "Transform.h"

class Abs_CameraObject;
class Abs_Abstract;

struct CubeKeyEvent {
	int code;
	bool alt = false, control = false, shift = false, system = false;
};

struct CubeMousePos {
	CubePoint pos = CubePoint(0, 0);
	int x_absolute, y_absolute;
	bool left = false, middle = false, right = false;
};

class Session {
	public:
		void initSession();

		const static CoordStruct CameraLocationDefault; //	384, 384, 320
		float CameraOffestX = 0.0, CameraOffestY = 0.0;
		Abs_CameraObject *CameraObject = nullptr;
		CoordStruct CameraLocation = CameraLocationDefault;
		void CameraMove(DirectionType::Type Direction, float offest = 2.0f);
		void UpdateCameraOffest() { UpdateVm(CameraOffestX, CameraOffestY); }

		unsigned int MaxID = 0;
		unsigned int addObject(Abs_Abstract &src);
		unsigned int getMaxObjectID() { return MaxID; }

		void removeObject(Abs_Abstract *src);

		//	Lua Interface
		void LUA_CameraMove(unsigned short Direction, float offest = 2.0f);

		void setKeyEvent(sf::Event::KeyEvent s);
		void setMousePos_Ab();
		void setMousePos_Rl(sf::Event::MouseButtonEvent &e);
		void setMousePos_Rl(sf::Event::MouseMoveEvent &e);
		void updateMouseButtonStatus();
		void setMousePos_Press(sf::Event::MouseButtonEvent &e);
		void setMousePos_Release(sf::Event::MouseButtonEvent &e);
		CubeKeyEvent *getKeyEvent() { return &KeyData; }
		CubeMousePos *getMousePos() { return &MousePosData; }
		CubeKeyEvent KeyData;
		CubeMousePos MousePosData;
};

#include "Abstract.h"
#include "CameraObject.h"

#endif