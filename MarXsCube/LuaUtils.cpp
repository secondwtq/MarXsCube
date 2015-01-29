#include "Common.h"
#include "LuaUtils.h"
#include "ObjectManger.h"

#include "Transform.h"

#include "Techno.h"

//#include <windows.h>
#include <time.h>

namespace LuaUtils {

	namespace {
//		double _dqFreq;
		// _LARGE_INTEGER _timer_time;
	}

	void initLuaUtils() {LOGFUNC;
//		LARGE_INTEGER f;
//		QueryPerformanceFrequency(&f);
//		_dqFreq = (double)f.QuadPart / 1000.0;
	}

	void closeSession() {LOGFUNC;
		safe_session_close();
	}

	CoordStruct GetCoordFromPoint(CubePoint pt) {LOGFUNC;
		return obsTransform::GetWorldPos(pt);
	}

	double GetTime() {LOGFUNC;
//		_LARGE_INTEGER t;
//    	QueryPerformanceCounter(&t);
//   		return (t.QuadPart / _dqFreq);
        return 0;
	}

	double GetTime_Clock() {LOGFUNC;
   		return (double)(((double)clock() / (double)CLOCKS_PER_SEC) * 1000.0);
	}

	// gperf said it maybe very slow, so try static_cast
	//		also I heard that dynamic_cast is not encouraged to use in games.
	Abs_Techno *toTechno(Abs_Abstract *src) { return static_cast<Abs_Techno *>(src); }
	
	luabridge::LuaRef CreateObjectTable_Default(lua_State* L) {
		return luabridge::newTable(L); }

}