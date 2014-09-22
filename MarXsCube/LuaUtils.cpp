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
		TestManger::GetInstance().window->close();
	}

	CoordStruct GetCoordFromPoint(CubePoint pt) {LOGFUNC;
		return GetWorldPos(pt);
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

	Abs_Techno *toTechno(Abs_Abstract *src) { return dynamic_cast<Abs_Techno *>(src); }

}