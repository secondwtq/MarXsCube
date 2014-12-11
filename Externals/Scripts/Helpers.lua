-- this file is part of Project MarXsCube Prototype.

-- subproject:
--
--		Lunar?
--
-- basic support of components
--
-- including:
--
--		* several helper function to use in scripting game logic
--				including wrappers for MarXsCube->CoreHelpers
--
-- dependency:
--
--		* CubeCore->Interfaces->CoreHelpers
--
--	use in other modules:
--
--		Helpers = require 'Helpers'

--
--	changelog:
--
--		* 2014.10.4 PM initial commit.
--		* 2014.10.4 EVE refined.
--
--	http://github.com/secondwtq/MarXsCube

local __Helpers = { }

function __Helpers.texture(name)
	return CoreHelpers.texture(name)
end

function __Helpers.TechnoType_Techno(object)
	return object:getTechnoType()
end

function __Helpers.scriptType_Techno(object)
	return object:getTechnoType().ScriptType
end

function __Helpers.scriptType_TechnoRTTITable(table)
	return table.ScriptType
end

function __Helpers.RTTIID_table(table)
	return table.__RTTIID
end

function __Helpers.Techno_TechnoRTTIIDTable(table)
	return table.Techno
end

function __Helpers.TechnoType_TechnoRTTIIDTable(table)
	return table.TechnoType
end

local math = require 'math'
function __Helpers.coord_distance(op1, op2)
	return math.sqrt(math.pow(op1.x - op2.x, 2)+math.pow(op1.y - op2.y, 2)+math.pow(op1.z - op2.z, 2))
end

function __Helpers.vector2_distance(op1, op2)
	return math.sqrt(math.pow(op1[1]-op2[1], 2)+math.pow(op1[2]-op2[2], 2))
end

function __Helpers.vector3_distance(op1, op2)
	return math.sqrt(math.pow(op1[1]-op2[1], 2)+math.pow(op1[2]-op2[2], 2)+math.pow(op1[3]-op2[3], 2))
end

function __Helpers.vector2_len(op)
	return math.sqrt(math.pow(op[1], 2)+math.pow(op[2], 2))
end

function __Helpers.vector2_nom(op)
	local len = __Helpers.vector2_len(op)
	return { op[1]/len, op[2]/len }
end

function __Helpers.vector2_offset(op1, op2)
	return { op1[1]-op2[1], op1[2]-op2[2] }
end

function __Helpers.vector2_dot(op1, op2)
	return op1[1]*op2[1]+op1[2]*op2[2]
end

function __Helpers.local_vector2(local_pos, local_dir, target)
	local offset = __Helpers.vector2_offset(target, local_pos)
	local local_dir_nom = __Helpers.vector2_nom(local_dir)

	local x = __Helpers.vector2_dot(offset, {local_dir_nom[2], -local_dir_nom[1]})
	local y = __Helpers.vector2_dot(offset, local_dir_nom)

	return { x, y }
end

function __Helpers.unpack_coord2(coord)
	return { coord.x, coord.y }
end

function __Helpers.unpack_coord3(coord)
	return { coord.x, coord.y, coord.z }
end

function __Helpers.sign(src)
	if src > 0 then return 1 elseif src < 0 then return -1 else return 0 end
end

function __Helpers.rad_from_vector2(vec2)
	local offsq = math.sqrt(vec2[1]*vec2[1]+vec2[2]*vec2[2])
	local ret = math.acos(vec2[1]/offsq)
	if vec2[2] > 0 then ret = 2*math.pi - ret end
	return ret
end

function __Helpers.absoffset_rad(op1, op2)
	local ret = math.abs(op1 - op2)
	if ret > math.pi then
		ret = -1*(ret - 2*math.pi)
	end
	return ret
end

__Helpers.tblinsert = table.insert

return __Helpers
