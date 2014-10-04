#ifndef CUBE_E_RENDERELE_H
#define CUBE_E_RENDERELE_H

#include "Common.h"
#include "SFML.h"
#include "Abstract.h"

class RenderElement {
	public:
		bool Enabled = true;
	
		CoordStruct offset = CoordStruct(0, 0, 0);
		Vector4DT<float> colorMultiply = Vector4DT<float>(1, 1, 1, 1);
		int direction = 0;
		int direction_offset = 0;
		bool UseShadowProjection = false;
		Vector3DT<double> ProjectionVector = ShadowProjectionVector; //	Vector3DT<double>(0.1239851454748, -0.7748446592171, 0.61987572737); //	Vector3DT<double>(4, -25, 20);

		inline void Render(CoordStruct &&loc) {
			if (this->Enabled)
				this->_Render_Overload(loc);
		}

		virtual ~RenderElement() { }
	
protected:
	virtual void _Render_Overload(CoordStruct &loc) = 0;
	
};

class RenderElement_DirectionedStatic : public RenderElement {
	public:
		TextureAtlas *texture = nullptr;
		sf::Sprite renderSprite;
		int frameCount = 32;
	
		~RenderElement_DirectionedStatic() {
			delete this->texture;
		}

		RenderElement_DirectionedStatic(TextureAtlas *_texture, int countFrame = 32) : texture(_texture), frameCount(countFrame) {LOGFUNC; }

		inline int getCurrentFrame() {LOGFUNC; return getDirFrameNum(direction, frameCount); }

		static RenderElement_DirectionedStatic *createElement(TextureAtlas *texture, int countFrame = 32) {LOGFUNC;
			return new RenderElement_DirectionedStatic(texture, countFrame);
		}
	
protected:
	void _Render_Overload(CoordStruct &loc);
};

class RenderElement_FramedStatic : public RenderElement {
	public:
		TextureAtlas *texture = nullptr;
		sf::Sprite renderSprite;
		void Render(CoordStruct &&loc);
		int currentFrame = 0;

		RenderElement_FramedStatic(TextureAtlas *_texture) : texture(_texture) {LOGFUNC; }
	
		~RenderElement_FramedStatic() {
			delete this->texture;
		}

		void setCurrentFrame(int frame) { currentFrame = frame; }

		static RenderElement_FramedStatic *createElement(TextureAtlas *texture) {LOGFUNC;
			return new RenderElement_FramedStatic(texture);
		}
	
protected:
	void _Render_Overload(CoordStruct &loc);
};

class RenderElement_FramedDynamic : public RenderElement {
public:
	TextureAtlas *texture = nullptr;
	
	USIZE frame_count = 1;
	USIZE current_frame = 1;
	
	sf::Sprite renderSprite;
	
	RenderElement_FramedDynamic(TextureAtlas *_texture, USIZE _frame_count)
		: texture(_texture), frame_count(_frame_count) {LOGFUNC; }
	
	~RenderElement_FramedDynamic() {
		delete this->texture;
	}
	
	static RenderElement_FramedDynamic *create(TextureAtlas *texture, USIZE frame_count = 1) {LOGFUNC;
		return new RenderElement_FramedDynamic(texture, frame_count);
	}

protected:
	void _Render_Overload(CoordStruct &loc);
};

template <class T>
inline void SetProjectionLocation_General(T *element, CoordStruct& loc) {
	if (!element->UseShadowProjection)
		element->renderSprite.setPosition(obsTransform::GetViewPos(loc+element->offset));
	else {
		auto _loc = loc + element->offset;
		_loc = CoordStruct(element->ProjectionVector.z*_loc.x+element->ProjectionVector.x*_loc.z, element->ProjectionVector.z*_loc.y+element->ProjectionVector.y*_loc.z, 0);
		element->renderSprite.setPosition(obsTransform::GetViewPos(_loc));
	}
}

#endif