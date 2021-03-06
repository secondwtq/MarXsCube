#ifndef CUBE_E_RENDERELE_H
#define CUBE_E_RENDERELE_H

#include "Common.h"
#include "SFML.h"
#include "Abstract.h"
#include "SFLineShape.h"
#include "SilconSprite.h"

#include "CubeTransform.h"

class RenderElementsContainer;

class RenderElement {
	public:
		bool Enabled = true;
	
		RenderElement() { }
	
		RenderElementsContainer *parent = nullptr;
		CoordStruct offset = CoordStruct(0, 0, 0);
		Vector4DT<float> colorMultiply = Vector4DT<float>(1, 1, 1, 1);
		int direction = 0;
		int direction_offset = 0;
		bool UseShadowProjection = false;
		int z_index = 0;
		Vector3DT<double> ProjectionVector = ShadowProjectionVector; //	Vector3DT<double>(0.1239851454748, -0.7748446592171, 0.61987572737); //	Vector3DT<double>(4, -25, 20);

		inline void Update(CoordStruct &&loc) {
			if (this->Enabled)
				this->_update_overload(loc); }

		virtual ~RenderElement() { }
	
protected:
	virtual void _update_overload(CoordStruct &loc) = 0;
};

class RenderElement_DirectionedStatic : public RenderElement {
	public:
		TextureAtlas *texture = nullptr;
		SilconSprite sprite;
		int frameCount = 32;
	
		~RenderElement_DirectionedStatic() {
			delete this->texture; }
	
		int getCurrentFrame();

		RenderElement_DirectionedStatic(TextureAtlas *_texture, int countFrame = 32)
			: RenderElement(), texture(_texture), frameCount(countFrame) {
			Silcon::Manger->register_sprite(&this->sprite);
			this->sprite.set_texture(*this->texture);
		}

		static RenderElement_DirectionedStatic *createElement(TextureAtlas *texture, int countFrame = 32) {LOGFUNC;
			return new RenderElement_DirectionedStatic(texture, countFrame);
		}
	
	protected:
	
		void _update_overload(CoordStruct &loc);
};

// In Silcon NT you can only specific texture for RenderElements at creation
//	if you want to set it manually, you should update the Sprite manually too.
//		I won't do this for you.
class RenderElement_FramedStatic : public RenderElement {
	public:
		TextureAtlas *texture = nullptr;
		SilconSprite sprite;
		void Render(CoordStruct &&loc);
		int currentFrame = 0;

		RenderElement_FramedStatic(TextureAtlas *_texture) : RenderElement(), texture(_texture) {
			Silcon::Manger->register_sprite(&this->sprite); this->sprite.set_texture(*this->texture);
		}
	
		~RenderElement_FramedStatic() {
			delete this->texture;
		}

		void setCurrentFrame(int frame) { currentFrame = frame; }

		static RenderElement_FramedStatic *createElement(TextureAtlas *texture) {LOGFUNC;
			return new RenderElement_FramedStatic(texture);
		}
	
protected:
	void _update_overload(CoordStruct &loc);
};

class RenderElement_FramedDynamic : public RenderElement {
public:
	TextureAtlas *texture = nullptr;
	
	USIZE frame_count = 1;
	USIZE current_frame = 1;
	
	sf::Sprite renderSprite;
	
	RenderElement_FramedDynamic(TextureAtlas *_texture, USIZE _frame_count)
		: RenderElement(), texture(_texture), frame_count(_frame_count) { assert(false); }
	
	~RenderElement_FramedDynamic() {
		delete this->texture;
	}
	
	static RenderElement_FramedDynamic *create(TextureAtlas *texture, USIZE frame_count = 1) {LOGFUNC;
		return new RenderElement_FramedDynamic(texture, frame_count);
	}

protected:
	void _update_overload(CoordStruct &loc);
};

class RenderElement_InternalLine : public RenderElement {
public:
	
	// inherited CoordStruct offset.
	CoordStruct point1 = CoordStruct(0, 0, 0);
	CoordStruct point2 = CoordStruct(0, 0, 0);
	Vector4DT<float> color = Vector4DT<float>(0, 0, 0, 1);
	float thickness = 0;
	sf::LineShape shape;
	
	RenderElement_InternalLine(const CoordStruct &pt1, const CoordStruct &pt2, const Vector4DT<float> &_color) :
		RenderElement(), point1(pt1), point2(pt2), color(_color), shape(Vector2f(0, 0), Vector2f(0, 0)) {
			this->shape.setThickness(10);
	}
	
	~RenderElement_InternalLine() { assert(false); }
	
	static RenderElement_InternalLine *create(const CoordStruct *pt1, const CoordStruct *pt2, const Vector4DT<float> *color) {
		return new RenderElement_InternalLine(*pt1, *pt2, *color);
	}
	
protected:
	
	void _update_overload(CoordStruct &loc) { }
	
};

template <class T>
inline void SetProjectionLocation_General(T *element, CoordStruct& loc) {
	if (!element->UseShadowProjection)
		element->renderSprite.setPosition(CubeTransform::view_pos(loc+element->offset));
	else {
		auto _loc = loc + element->offset;
		_loc = CoordStruct(element->ProjectionVector.z*_loc.x+element->ProjectionVector.x*_loc.z, element->ProjectionVector.z*_loc.y+element->ProjectionVector.y*_loc.z, 0);
		element->renderSprite.setPosition(CubeTransform::view_pos(_loc));
	}
}

#include "RenderElementsContainer.h"

#endif