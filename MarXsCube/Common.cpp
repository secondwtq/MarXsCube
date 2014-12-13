#include "Common.h"
#include "SFML.h"
#include "ObjectManger.h"
using namespace sf;

int DIVS = 64;
int WIDTH = 1024, HEIGHT = 768;
int FPSLimit = 30;
int divPhysics = 240;
double PI = 3.1415926535898;
double TransformScaleFactor = 0.6484375;

CubeTimer *CubeTimer::createCubeTimer(int Interval, int LoopCount) {
	LOGFUNC;
	return new CubeTimer(Interval, LoopCount);
}

int CubeTimer::Update() {
	LOGFUNC;
	int ret = current;
	if (Enabled && !TimerEnded)
		if (!Paused) {
			if (++current >= Interval) {
				current = 0;
				LoopTimed++;
			}
			if (LoopCount != -1)
				if ((int)LoopTimed >= LoopCount) {
					Enabled = false;
					TimerEnded = true;
				}
		}
	return ret;
}

 void CubeTimer::Reset(int interval, int LoopCount) {
 	LOGFUNC;
	Paused = Enabled = false;
	Interval = interval;
	this->LoopCount = LoopCount;
	TimerEnded = false;
	LoopTimed = 0;
	current = 0;
}

bool CubeTimer::SwitchStart() {
	LOGFUNC;
	Enabled = !Enabled;
	Paused = false;
	LoopTimed = 0;
	TimerEnded = !Enabled;
	return Enabled;
}

bool CubeTimer::SwitchPause() {
	LOGFUNC;
	if (Enabled && !Paused) Paused = true;
	else if (Enabled) Paused = false;
	return Paused;
}

void CubeTimer::ResetToEnd() {
	LOGFUNC;
	Paused = Enabled = false;
	TimerEnded = true;
	LoopTimed = 0;
}

TextureAtlas::TextureAtlas(const char *filename, unsigned short num, char hori, char vert) : TexNum(num), TexDist(hori, vert) {
	LOGFUNC;
	texture.loadFromFile(filename);
	auto size = texture.getSize();
	TexSizeTotal = Vector2DT<unsigned int>(size.x, size.y);
	TexSizeSingle = Vector2DT<unsigned int>(size.x / hori, size.y / vert);
	genAreas();
}

sf::IntRect TextureAtlas::_getArea(unsigned short idx) {
	LOGFUNC;
	char texHori = idx % TexDist.x, texVert = idx / TexDist.x;
	// printf("%d %d %d %d %d\n", idx, texHori * TexSizeSingle.x, texVert * TexSizeSingle.y, TexSizeSingle.x, TexSizeSingle.y);
	return sf::IntRect(texHori * TexSizeSingle.x, texVert * TexSizeSingle.y, TexSizeSingle.x, TexSizeSingle.y);
}

void TextureAtlas::genAreas() {
	LOGFUNC;
	for (int i = 0; i < TexNum; i++)
		TextureAreas.push_back(_getArea(i)); }

sf::Vector2f TextureAtlas::CenterPivot(int offsetX, int offsetY) {
	LOGFUNC;
	return sf::Vector2f(TexSizeSingle.x/2+offsetX, TexSizeSingle.y/2+offsetY); }

sf::Vector2f TextureAtlas::CenterPivot(sf::Transformable &src, int offsetX, int offsetY) {
	LOGFUNC;
	auto a = CenterPivot(offsetX, offsetY); src.setOrigin(a); return a; }

void TextureAtlas::setArea_(sf::Sprite &src, int frameIdx) {
	LOGFUNC;
	int texIdx = 0;
	for (int i = 0; i <= extraCount; i++)
		if (frameIdx <= frames[i]) {
			texIdx = i;
			break;
		}
	if (texIdx == 0) {
		src.setTexture(texture);
		src.setTextureRect(getArea(frameIdx));
	} else {
		src.setTexture(extras[texIdx-1]);
		src.setTextureRect(getArea(frameIdx-frames[texIdx-1]));
	}
}