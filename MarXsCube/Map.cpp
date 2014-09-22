#include "Common.h"
#include "Map.h"
#include "Cell.h"
#include "Session.h"

Map *Map::instance = new Map();

void Map::AllocateCells() {
	for (auto cell : Cells) {
		ObjectManger::GetInstance().addObject(*cell);
		RenderLayerManger::GetInstance().Layers[RenderLayerType::Type::LAYER_CELL].addObject(*cell);
		RenderLayerManger::GetInstance();
	}
}

void Map::CreateEmptyMap(int x, int y) {
	size = CubePoint(x, y);
	for (int i = 0; i < x; i++)
		for (int j = -y/2; j < y/2; j++)
			Cells.push_back(new Abs_Cell(i, j));
	AllocateCells();
}

Abs_Cell *Map::GetCellAt(CellStruct &pos) {
	for (auto cell : Cells)
		if (cell->LocCell == pos) { return cell; }
	return nullptr;
}

Abs_Cell *Map::GetCellAt_Pos(int x, int y) {
	auto t = CellStruct(x, y);
	return GetCellAt(t);
}

CellStruct Map::CellFromCoord(CoordStruct &pos) {
	return CellStruct((pos.x+DIVS/2) / DIVS, (pos.y+(DIVS/2) * ((pos.y < 0) ? -1 : 1)) / DIVS);
}
