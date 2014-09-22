#ifndef CUBE_E_MAP_H
#define CUBE_E_MAP_H

#include <vector>
#include "Cell.h"
using namespace std;

class Map {
	public:
		static Map *instance;
		static inline Map &GetInstance() {LOGFUNC; return *instance; }

		CubePoint size = CubePoint(0, 0);

		vector<Abs_Cell *> Cells;

		void AllocateCells();
		void CreateEmptyMap(int x, int y);

		Abs_Cell *GetCellAt(CellStruct &pos);
		Abs_Cell *GetCellAt_Pos(int x, int y);
		static CellStruct CellFromCoord(CoordStruct &pos);
};

#endif