open = { }
close = { }

node = {
	cell = nil,
	father = nil,
	G = 0, H = 0, F = 0,
	InsideOpen = true,
	Passable = false
}

cellopz = { }

function node.new(cell, obj, father)
	local u = { }
	for k, v in pairs(node) do u[k] = v end
	u.cell = cell
	if (cell ~= nil) then u.Passable = cell:PassableTo(obj) end
	if (father ~= nil) then u.father = father end
	-- return setmetatable(u, getmetatable(s))
	return u
end

function get_H(src, destCell)
	return (math.abs(src.LocCell.x - destCell.LocCell.x) + math.abs(src.LocCell.y - destCell.LocCell.y))
end

function CellFromOffset(z, x, y)
	local cells = z.LocCell
	local pos = Utility.CellStruct(cells.x+x, cells.y+y)
	local cell = Objects.Map.GetInstance():GetCellAt(pos)
	return cell
end

function Pathfinding_init()
	for i = -2, 62 do
		cellopz[i] = { }
	end
	local c = Objects.Map.GetInstance():GetCellAt(Utility.CellStruct(0, 0))
	for i = -2, 62 do
		for j = -32, 32 do
			cellopz[i][j] = Objects.Map.GetInstance():GetCellAt(Utility.CellStruct(i, j))
		end
	end
end

function InsideOpen(cell)
	for k, v in pairs(open) do
		if (v.cell.LocCell.x == cell.cell.LocCell.x) and (v.cell.LocCell.y == cell.cell.LocCell.y) and v.InsideOpen == true then return v end
	end
	return nil
end

function InsideClose(cell)
	for k, v in pairs(close) do
		if (v.cell.LocCell.x == cell.cell.LocCell.x) and (v.cell.LocCell.y == cell.cell.LocCell.y) then return v end
	end
	return nil
end

function getMinF(def)
	local ret = open[1]
	for k, v in ipairs(open) do
		if (ret.InsideOpen == false and v.InsideOpen == true) then ret = v end
		if (v.F < ret.F) and (v.InsideOpen == true) then ret = v end
	end
	return ret
end

function find_rec(obj, father, dest)
	table.insert(close, father)
	if father.cell.LocCell.x == dest.LocCell.x and father.cell.LocCell.y == dest.LocCell.y then return end
	
	-- local getCell = function(x, y) return node.new(CellFromOffset(father.cell, x, y), obj) end
	local getCell = function(x, y)
		return node.new(cellopz[father.cell.LocCell.x+x][father.cell.LocCell.y+y], obj)
	end

	local _childNodes = { getCell(-1, 0), getCell(1, 0), getCell(0, 1), getCell(0, -1), getCell(-1, -1), getCell(1, 1), getCell(-1, 1), getCell(1, -1) }
	if (_childNodes[1].Passable == false) and (_childNodes[3].Passable == false) then _childNodes[7].Passable = false end
	if (_childNodes[2].Passable == false) and (_childNodes[3].Passable == false) then _childNodes[6].Passable = false end
	if (_childNodes[1].Passable == false) and (_childNodes[4].Passable == false) then _childNodes[5].Passable = false end
	if (_childNodes[2].Passable == false) and (_childNodes[4].Passable == false) then _childNodes[8].Passable = false end
	local childNodes = { }
	for k, v in ipairs(_childNodes) do if v.Passable == true then table.insert(childNodes, v) end end

	for k, node in ipairs(childNodes) do
		closeNode = InsideClose(node)
		if closeNode == nil then
			local better = false
			node.G = father.G+1
			node.H = get_H(node.cell, dest)
			node.F = node.G+node.H

			openNode = InsideOpen(node)
			if openNode == nil then
				table.insert(open, node)
				better = true
			elseif node.G < openNode.G then better = true
			else better = false end
			if (better == true) then node.father = father end
		end
	end
	father.InsideOpen = false
	find_rec(obj, getMinF(), dest)
end

function find(obj, src, dest)
	close = { }
	open = { }
	local startT = Utility.GetTime()
	local startT2 = Utility.GetTime_Clock()

	if src:PassableTo(obj) and dest:PassableTo(obj) then
		local srcNode = node.new(src, nil, obj)
		srcNode.H = get_H(srcNode.cell, dest)
		srcNode.F = srcNode.G+srcNode.H
		table.insert(open, srcNode)
		find_rec(obj, srcNode, dest)
		local c = InsideClose(node.new(dest))
		while c.father ~= nil do
			c.cell.ShowCenter = true
			c = c.father
		end
	end
	print(Utility.GetTime() - startT)
	print(Utility.GetTime_Clock() - startT2)
end