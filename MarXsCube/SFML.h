#ifndef CUBE_E_SFML_H
#define CUBE_E_SFML_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

#include "Common.h"

struct TextureAtlasArg {
	std::string filename = "";
	unsigned short num = 1;
	Vector2DT<char> TexDist = Vector2DT<char>(0, 0);

	TextureAtlasArg(const char *_filename, unsigned short _num=1, char hori=1, char vert=1) : filename(_filename), num(_num), TexDist(hori, vert) {LOGFUNC; }
};

class RenderElement;

void InternalDraw_Ext(sf::Drawable &obj);
void InternalDraw_Ext(RenderElement &objArg, sf::Drawable &obj);

class TextureAtlas {
	public:
		sf::Texture texture;
		unsigned short TexNum = -1;
		int totalTexNum = -1;
		bool useExtra = false;
		int extraCount = 0;
		Vector2DT<char> TexDist = Vector2DT<char> (0, 0);
		Vector2DT<unsigned int> TexSizeTotal = Vector2DT<unsigned int>(0, 0);
		Vector2DT<unsigned int> TexSizeSingle = Vector2DT<unsigned int>(0, 0);
		std::vector<sf::IntRect> TextureAreas;
		std::vector<sf::Texture> extras;
		std::vector<int> frames;

		TextureAtlas() {LOGFUNC; }

		TextureAtlas(TextureAtlasArg &args) : TextureAtlas(args.filename.c_str(), args.num, args.TexDist.x, args.TexDist.y) {LOGFUNC; }

		TextureAtlas(const char *filename, unsigned short num=1, char hori=1, char vert=1);

		operator const sf::Texture &() const {LOGFUNC; return texture; }
		operator const sf::Texture *() const {LOGFUNC; return &texture; }

		sf::Vector2f CenterPivot(int offsetX=0, int offsetY=0);
		sf::Vector2f CenterPivot(sf::Transformable &src, int offsetX=0, int offsetY=0);

		void setArea_(sf::Sprite &src, int frameIdx);

		inline sf::IntRect &getArea(unsigned short frameIdx) { return TextureAreas[frameIdx]; }
		inline sf::IntRect &setArea(sf::Sprite &src, unsigned short frameIdx) { src.setTextureRect(getArea(frameIdx)); return TextureAreas[frameIdx]; }
		inline sf::Vector2u getSize() { return texture.getSize(); }

		void genAreas();

		sf::IntRect _getArea(unsigned short idx);
};

#include "RenderElement.h"

#endif