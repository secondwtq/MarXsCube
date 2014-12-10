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

__Helpers.tblinsert = table.insert

return __Helpers
