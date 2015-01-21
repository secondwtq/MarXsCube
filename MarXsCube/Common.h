#ifndef CUBE_E_COMMON_H
#define CUBE_E_COMMON_H

#define USE_PARALLEL_SOLVER 1
#define USE_PARALLEL_DISPATCHER 1
#define USE_PTHREADS

// #define _GLIBCXX_DEBUG 1

#include <stdio.h>
#include "Debug.h"

typedef size_t USIZE;
typedef unsigned int GLIDX;
// typedef unsigned int size_t;

extern int DIVS;
extern int WIDTH, HEIGHT;
extern int FPSLimit;
extern int divPhysics;
extern double PI;
extern double TransformScaleFactor;

//	RTTIType
#define RTTIType_Abstract 0
#define RTTIType_Cell 1
#define RTTIType_Object 2
#define RTTIType_AbstractType 3
#define RTTIType_ObjectType 4
#define RTTIType_Anim 5
#define RTTIType_AnimType 6
#define RTTIType_Techno 7
#define RTTIType_TechnoType 8
#define RTTIType_TechnoRTTIIDTable 9

/*
	Render Layer

	0 - UI Elements
	1 - Objects
	2 - Cell
 */

class RenderLayerType {
public:
	typedef unsigned int Value;
	static const size_t Count = 4;
	enum Type {
		LAYER_NONE = 0x0,
		LAYER_CELL = 0x1,
		LAYER_OBJECT = 0x2,
		LAYER_UI = 0x3
	};
};

class DirectionType {
public:
	typedef unsigned int Value;
	static const size_t Count = 8;
	enum Type {
		North = 0x0,
		NorthEast = 0x1,
		East = 0x2,
		SouthEast = 0x3,
		South = 0x4,
		SouthWest = 0x5,
		West = 0x6,
		NorthWest = 0x7,
	};
};

template <typename T>
class VectorT {

};

template <typename T>
class Vector2DT : public VectorT<T> {
	public:
		T x, y;
		Vector2DT() : x(), y() { }
		Vector2DT(T _x, T _y) : x(_x), y(_y) { }

		bool operator==(const Vector2DT<T> &o) { return ((this->x == o.x) && (this->y == o.y)); }
		Vector2DT<T> &operator=(const Vector2DT<T> &o) { this->x = o.x, this->y = o.y; return *this; }
		Vector2DT<T> &operator+=(const Vector2DT<T> &o) { x+=o.x, y+=o.y; return *this; }
		Vector2DT<T> operator+(const Vector2DT<T> &o) { Vector2DT<T> ret = *this; ret += o; return ret; }

};

template <typename T>
class Vector3DT : public VectorT<T> {
	public:
		T x, y, z;
		Vector3DT(T _x, T _y, T _z) : x(_x), y(_y), z(_z) { }

		Vector3DT<T> &operator+=(const Vector3DT<T> &o) { x+=o.x, y+=o.y; z+=o.z; return *this; }
		Vector3DT<T> operator+(const Vector3DT<T> &o) { Vector3DT<T> ret = *this; ret += o; return ret; }
		Vector3DT<T> &operator=(const Vector3DT<T> &o) { this->x = o.x, this->y = o.y, this->z = o.z; return *this; }
};

template <typename T>
class Vector4DT : public VectorT<T> {
	public:
		T x, y, z, w;
		Vector4DT(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) { }

		Vector4DT<T> &operator+=(const Vector4DT<T> &o) { x+=o.x, y+=o.y; z+=o.z; w+=o.w; return *this; }
		Vector4DT<T> operator+(const Vector4DT<T> &o) { Vector4DT<T> ret = *this; ret += o; return ret; }
		Vector4DT<T> &operator=(const Vector4DT<T> &o) { this->x = o.x, this->y = o.y, this->z = o.z; this->w = o.w; return *this; }
};

using CellStruct = Vector2DT<int>;
using CoordStruct = Vector3DT<int>;
using CubePoint = Vector2DT<int>;
using Homogeneous4D = Vector4DT<float>;
using Float3D = Vector3DT<float>;

class CubeTimer {
	public:
		bool Enabled = false;
		bool Paused = false;
		bool TimerEnded = false;
		int current = 0;
		int Interval = -1;
		int LoopCount = -1;
		int LoopTimed = 0;

		void ResetToEnd();

		void Reset(int interval, int LoopCount = 1);

		bool SwitchStart();

		bool SwitchPause();

		inline int GetCurrent() { return current; }
		inline int GetLoopedCount() { return LoopTimed; }
		inline bool IsTimerEnded() { return TimerEnded; }
		inline char GetPercentage() { LOGFUNC; if (current >= 0 && Interval >= 0) return (int)((current / (double)Interval) * 100.0); else return 100; }
		int GetPercentage_LUA() { LOGFUNC; if (current >= 0 && Interval >= 0) return (int)((current / (double)Interval) * 100.0); else return 100; }
		bool IsTimerEnded_LUA() { return TimerEnded; }
		int GetCurrent_LUA() { return current; }

		int Update();

		CubeTimer(int _Interval, int _LoopCount) : Interval(_Interval), LoopCount(_LoopCount) { }

		static CubeTimer *createCubeTimer(int Interval, int LoopCount);
};

inline int getDirFrameNum(int deg, int totalNum = 32) { LOGFUNC;
	double r = deg % 360;
	r += (360.0 / totalNum) * 0.9;
	if (r < 0) r += 360.0;
	else if (r > 360.0) r -= 360.0;
	return (r / 360.0) * totalNum;
}

inline double torad(int degree) { return degree / (180.0 / PI); }

class ObjectDirection {
	public:
		unsigned short degree = 0;

		ObjectDirection(unsigned short defDeg = 0) : degree(defDeg) { }

		unsigned short getDirection() { LOGFUNC; return degree; }
		void setDirection(unsigned short deg) { LOGFUNC; degree = deg; }

		inline int getFrameNum(int totalNum = 32) { LOGFUNC; return getDirFrameNum(degree, totalNum); }
};

const Vector3DT<double> ShadowProjectionVector = Vector3DT<double>(0.2, -1.25, 1);

#endif