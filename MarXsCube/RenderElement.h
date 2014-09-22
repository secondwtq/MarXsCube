#ifndef CUBE_E_RENDERELE_H
#define CUBE_E_RENDERELE_H

#include "Common.h"
#include "SFML.h"

#include <unordered_map>

#include "Abstract.h"

class RenderElement {
	public:
		bool Enabled = true;
		CoordStruct offset = CoordStruct(0, 0, 0);
		Vector4DT<float> colorMultiply = Vector4DT<float>(1, 1, 1, 1);
		int direction = 0;
		bool UseShadowProjection = false;
		Vector3DT<double> ProjectionVector = ShadowProjectionVector; //	Vector3DT<double>(0.1239851454748, -0.7748446592171, 0.61987572737); //	Vector3DT<double>(4, -25, 20);

		virtual void Render(CoordStruct &&loc) = 0;

		virtual ~RenderElement() { }
};

class RenderElement_DirectionedStatic : public RenderElement {
	public:
		TextureAtlas *texture = nullptr;
		sf::Sprite renderSprite;
		void Render(CoordStruct &&loc);
		int frameCount = 32;

		RenderElement_DirectionedStatic(TextureAtlas *_texture, int countFrame = 32) : texture(_texture), frameCount(countFrame) {LOGFUNC; }

		inline int getCurrentFrame() {LOGFUNC; return getDirFrameNum(direction, frameCount); }

		static RenderElement_DirectionedStatic *createElement(TextureAtlas *texture, int countFrame = 32) {LOGFUNC;
			return new RenderElement_DirectionedStatic(texture, countFrame);
		}
};

class RenderElement_FramedStatic : public RenderElement {
	public:
		TextureAtlas *texture = nullptr;
		sf::Sprite renderSprite;
		void Render(CoordStruct &&loc);
		int currentFrame = 0;

		RenderElement_FramedStatic(TextureAtlas *_texture) : texture(_texture) {LOGFUNC; }

		void setCurrentFrame(int frame) { currentFrame = frame; }

		static RenderElement_FramedStatic *createElement(TextureAtlas *texture) {LOGFUNC;
			return new RenderElement_FramedStatic(texture);
		}
};

class RenderElementsContainer {
	public:
		std::unordered_multimap<int, RenderElement *> elements;
		Abs_Abstract *parent;
		void Update();
		RenderElementsContainer(Abs_Abstract *_parent) : parent(_parent) {LOGFUNC; }
		void setDirection(int degree);
		void insert(int Zidx, RenderElement *element) {LOGFUNC;
			elements.insert({Zidx, element});
		}

		~RenderElementsContainer() {
			for (auto i : elements)
				delete i.second;
		}
};

#endif