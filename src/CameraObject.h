#ifndef CUBE_E_CAMERA_H
#define CUBE_E_CAMERA_H

#include "Common.h"
#include "Object.h"
#include "Session.h"

class Abs_CameraObject : public Abs_Abstract {
	RenderLayerType::Type GetDefaultLayer() { return RenderLayerType::LAYER_UI; }

	CoordStruct GetCoord() const;
	void Update() { }
	void Render();
	int GetHeight() { return 0; }
};


#endif