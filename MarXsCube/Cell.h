#ifndef CUBE_E_CELL_H
#define CUBE_E_CELL_H

#include "Common.h"
#include "SFML.h"
using namespace sf;

#include "Abstract.h"
#include "ObjectManger.h"
#include <armadillo>

class Abs_Object;

class Abs_Cell : public Abs_Abstract {
	public:
		unsigned int WhatAmI() { return RTTIType_Cell; }

		CellStruct LocCell = Vector2DT<int>(0, 0);
		CoordStruct LocCoord = CoordStruct(0, 0, 0);
		Sprite renderSprite;
		TextureAtlas *renderTexture;
	TextureAtlas *texture1;
	TextureAtlas *texture2;
	TextureAtlas *texture3;
	TextureAtlas *texture4;
	TextureAtlas *texture_blend;
	
		Vector2i size;
		bool isSpecial = false;
		int height = 0;
		arma::vec::fixed<4> _coordObj;
		bool ShowCenter = false;

		Abs_Object *occupiedByObject = nullptr;

		void clearOccupyObject() { occupiedByObject = nullptr; }
		void setOccupyObject(Abs_Object *ptr) { occupiedByObject = ptr; }
		Abs_Object *getOccupyObject() {  return occupiedByObject; }

		Abs_Cell() : Abs_Cell(0, 0) { }

		static RenderLayerType::Type GetDefaultLayer() {LOGFUNC; return RenderLayerType::LAYER_CELL; }

		Abs_Cell(int x, int y, bool Special = false);

		CoordStruct GetCoord() const;

		void Render();

		int GetHeight() { return height; }

		bool PassableTo(Abs_Object *obj = nullptr);

		void Update();

		bool initialPerformOccTest(int xoff, int yoff, Abs_Object *Obj, int hei);
		Abs_Abstract *_initialPerformOccTest(int xoff, int yoff, int hei);

		Abs_Abstract *_occupiedByObject = nullptr;

		btCollisionShape *occTest_physhape = nullptr;
		btDefaultMotionState *occTest_mo = nullptr;
		btRigidBody *occTestBody = nullptr;
		Abs_Abstract *__occupiedByObject = nullptr;

		btDefaultMotionState *sphereTest_mo = nullptr;
};

#include "Object.h"

#endif