local __vhere = { }

-- local vector_2d_meta = { }

local vector_2d_table = {
	x = 0,
	y = 0
}

local vec3_table = {
	x = 0,
	y = 0,
	z = 0
}

local function vector2d(x, y)
	local ret = { }
	setmetatable(ret, vector_2d_table)
	ret.x, ret.y = x, y
	return ret
end

local function vec3(x, y, z)
	local ret = { }
	setmetatable(ret, vec3_table)
	ret.x, ret.y, ret.z = x, y, z
	return ret
end

function vector_2d_table.__add(op1, op2)
	if type(op1) == 'table' and type(op2) == 'table' then
		return vector2d(op1.x+op2.x, op1.y+op2.y)
	end
end

function vector_2d_table.__sub(op1, op2)
	if type(op1) == 'table' and type(op2) == 'table' then
		return vector2d(op1.x-op2.x, op1.y-op2.y)
	end
end

function vector_2d_table.__mul(op1, op2)
	if type(op1) == 'table' and type(op2) == 'table' then
		return vector2d(op1.x*op2.x, op1.y*op2.y)
	elseif type(op2) == 'number' then
		return vector2d(op1.x*op2, op1.y*op2)
	end
end

function vector_2d_table.__div(op1, op2)
	if type(op1) == 'table' and type(op2) == 'number' then
		return vector2d(op1.x/op2, op1.y/op2)
	end
end

function vector_2d_table.__eq(op1, op2)
	if type(op1) == 'table' and type(op2) == 'table' then
		return (op1.x == op2.x and op1.y == op2.y) or (op1.x == op2[1] and op1.y == op2[2])
	end
end

function vector_2d_table.__unm(op)
	return op * -1
end

function vector_2d_table.__index(table, key)
	if key == 1 then return table.x
	elseif key == 2 then return table.y
	elseif vector_2d_table[key] ~= nil then return vector_2d_table[key]
	else return nil end
end

function vector_2d_table.__newindex(table, key, value)
	if key == 1 then table.x = value
	elseif key == 2 then table.y = value
	else rawset(table, key, value)
	end
end

function vector_2d_table.__len(op)
	return 2
end

function vector_2d_table:dot(op)
	if type(op) == 'table' then
		return self.x*op.x+self.y*op.y
	elseif type(op) == 'number' then
		return vector2d(self.x*op, self.y*op)
	end
end

function vector_2d_table:len()
	return math.sqrt(math.pow(self.x, 2)+math.pow(self.y, 2))
end

function vector_2d_table:angle_cos(op)
	return self:dot(op) / (self:len() * op:len());
end

function vector_2d_table:str()
	return '<' .. tostring(self.x) .. ', ' .. tostring(self.y) .. '>'
end

function vector_2d_table:apply(foo)
	return vector2d(foo(self.x), foo(self.y))
end

function vector_2d_table:copy()
	return vector2d(self.x, self.y)
end

function vector_2d_table:nom()
	return (self:copy())/(self:len())
end

function vector_2d_table:reflect(nom)
	local n = nom:nom()
	return self - n * 2 * (self:dot(n))
end

function vector_2d_table:unpack()
	return self.x, self.y
end

-- setmetatable(vector_2d_table, vector_2d_meta)

local function get_unitvec_rad(rad)
	return vector2d(math.cos(rad), math.sin(rad))
end

local function dot(vec1, vec2)
	return vec1:dot(vec2)
end

local function print_vectable(src)
	for i, v in ipairs(src) do
		io.write(v:str())
		io.write(' ')
	end
	io.write('\n')
end

local function tbl2vec2(tb)
	return vector2d(tb[1], tb[2]) end

local function coord2vec2(coord)
	return vector2d(coord.x, coord.y) end

local function vec2coord(vec, z)
	if z == nil then z = 0 end
	return Utility.CoordStruct(vec.x, vec.y, z)
end

local function vec2glm3(vec, z)
	if z == nil then z = 0 end
	return Utility.glm.gvec3(vec.x, vec.y, z)
end

-- local function coord2vec2(tb)
-- 	return vector2d(tb[1], tb[2]) end

__vhere.vector2d = vector2d

__vhere.tbl2vec2 = tbl2vec2
__vhere.coord2vec2 = coord2vec2
__vhere.vec2coord = vec2coord
__vhere.vec2glm3 = vec2glm3
__vhere.get_unitvec_rad = get_unitvec_rad
__vhere.dot = dot
__vhere.print_vectable = print_vectable

return __vhere