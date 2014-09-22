open = { }
close = { }

node = {
	cell = nil,
	father = nil,
	G = 0, H = 0, F = 0,
	InsideOpen = false;
}

function node.new(cell, father)
	local u = { }
	for k, v in pairs(node) do u[k] = v end
	u.cell = cell
	if (father ~= nil) then u.father = father end
	-- return setmetatable(u, getmetatable(s))
	return u
end

function get_H(src, destCell)
	return (math.abs(src.LocCell.x - destCell.LocCell.x) + math.abs(src.LocCell.y - destCell.LocCell.y))
end

function printTable(tb)
	for k, v in ipairs(tb) do print(k, v) end
end

function CellFromOffset(z, x, y)
	local cells = z.LocCell
	local pos = Utility.CellStruct(cells.x+x, cells.y+y)
	local cell = Objects.Map.GetInstance():GetCellAt(pos)
	return cell
end

function InsideOpen(cell)
	for k, v in pairs(open) do
		if ((v.cell.LocCell.x == cell.cell.LocCell.x) and (v.cell.LocCell.y == cell.cell.LocCell.y)) then return true
	end
	return false
end

function getMinF(def, obj)
	local ret = def
	for k, v in pairs(open) do
		if (v.F < ret.F and v.InsideOpen == false and v.cell:PassableTo(obj) == true) or (ret.cell:PassableTo(obj) == false) then ret = v end
	end
	return ret
end

function find_rec(obj, father, dest)
	local getCell = function(x, y)
						local ret = node.new(CellFromOffset(father.cell, x, y), father)
						if ret then return ret else return nil end
					end

	local cell1 = getCell(-1, 0)
	local cell2 = getCell(1, 0)
	local cell3 = getCell(0, 1)
	local cell4 = getCell(0, -1)
	local cell5 = getCell(-1, -1)
	local cell6 = getCell(1, 1)
	local cell7 = getCell(-1, 1)
	local cell8 = getCell(1, -1)

	local parse = function(c)
		if c.cell ~= nil then
			if c.cell:PassableTo(obj) == true then
				c.G = father.G+1
				c.H = get_H(c.cell, dest)
				c.F = c.G+c.H
				print("Pushing", c.cell.LocCell.x, c.cell.LocCell.y)
				table.insert(open, c)
			end
		end
	end

	parse(cell1)
	parse(cell2)
	parse(cell3)
	parse(cell4)
	parse(cell5)
	parse(cell6)
	parse(cell7)
	parse(cell8)

	local cel
	local getMin = function()
				if cell1.cell ~= nil then cel = getMinF(cell1, obj) return end
				if cell2.cell ~= nil then cel = getMinF(cell2, obj) return end
				if cell3.cell ~= nil then cel = getMinF(cell3, obj) return end
				if cell4.cell ~= nil then cel = getMinF(cell4, obj) return end
				if cell5.cell ~= nil then cel = getMinF(cell5, obj) return end
				if cell6.cell ~= nil then cel = getMinF(cell6, obj) return end
				if cell7.cell ~= nil then cel = getMinF(cell7, obj) return end
				if cell8.cell ~= nil then cel = getMinF(cell8, obj) return end
			end
	getMin()
	print("Close Pushing", cel.cell.LocCell.x, cel.cell.LocCell.y)
	table.insert(close, cel)
	if cell1.cell ~= nil then  cell1.InsideOpen = true end
	if cell2.cell ~= nil then  cell2.InsideOpen = true end
	if cell3.cell ~= nil then  cell3.InsideOpen = true end
	if cell4.cell ~= nil then  cell4.InsideOpen = true end
	if cell5.cell ~= nil then  cell5.InsideOpen = true end
	if cell6.cell ~= nil then  cell6.InsideOpen = true end
	if cell7.cell ~= nil then  cell7.InsideOpen = true end
	if cell8.cell ~= nil then  cell8.InsideOpen = true end
	if cel.cell.LocCell.x == dest.LocCell.x and cel.cell.LocCell.y == dest.LocCell.y then return end
	find_rec(obj, cel, dest)
end

function find(obj, src, dest)
	close = { }
	open = { }
	if src:PassableTo(obj) and dest:PassableTo(obj) then
		local srcNode = node.new(src, nil)
		local a = { }
		table.insert(close, srcNode)
		find_rec(obj, srcNode, dest)
		for i, v in ipairs(close) do
			v.cell.ShowCenter = true
		end
	end
end