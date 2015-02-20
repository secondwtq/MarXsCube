Import("mapdef.lua")

Helpers = require 'Helpers'

-- global variables
OBJ_DOTS = { }
OBJ_EDGES = { }

TECHNO_SELECTED = nil

TERRAIN_CHUNKS = { }
CURRENT_TERRAIN_TILE = 1

function acheroncube_test()

	local start = Utility.CubePoint(0, 0)
	local eend = Utility.CubePoint(1024, 0)
	print(Utility.GetTime_Clock())
	local vec = Acheron.find_path_async(start, eend, function (vec) print('path got', Utility.GetTime_Clock()) print(vec:size()) end)
	print(Utility.GetTime_Clock())

end

function grit_find_path(start, eend)
	local a, b = Utility.CubePoint(start[1], start[2]), Utility.CubePoint(eend[1], eend[2])
	local vec = Grit.instance():find_path(a, b)
	local len, ret = vec:size(), { }
	-- print('path length', len)
	for i = 0, len-1 do
		local pos = vec:at(i)
		-- print(pos.x, pos.y)
	end

	for i = len-1, 1, -1 do
		local pos = vec:at(i-1)
		table.insert(ret, { pos.x, pos.y, 0 })
	end

	return ret, Utility.Path.pathway_from_gritpath(vec)
end

-- run when game started
function Functions.TestManger_onTestInit()

	Objects.Map.GetInstance():CreateEmptyMap(60, 50) -- create map

	for i, chunk in ipairs(map.default.chunks) do
		local terrain = Tesla.TeslaChunkObject.create()
		terrain:location(Utility.CoordStruct(unpack(chunk.location)))
		terrain:set_heightfield(Helpers.texture(chunk.heightfield))
		terrain:set_tileset(Helpers.texture(chunk.tileset))
		terrain:load_objfile(chunk.source, chunk.bullet_model)
		terrain:load_shader()
		terrain:create_bullet()
		terrain:load_buffer()
		Generic.TeslaManger():add_chunk(terrain)
		table.insert(TERRAIN_CHUNKS, terrain)
	end

	-- local background = ModEnvironment.Functions.createTechno(OBJECTS.SATELITE_BG, Utility.CoordStruct(64*27+56, -36, 0))
	-- background.temp_ZOffset = true
	local cycle = ModEnvironment.Functions.createTechno(OBJECTS.TESTTECHNO, Utility.CoordStruct(256, 0, 64), true)
	local rail = ModEnvironment.Functions.createTechno(OBJECTS.TESTTECHNO_PHY, Utility.CoordStruct(256, 256, 128), true)
	local building = ModEnvironment.Functions.createTechno(OBJECTS.TESTBUILDING, Utility.CoordStruct(512, 384, 0), true)
	-- local rail2 = ModEnvironment.Functions.createTechno(OBJECTS.TESTTECHNO_PHY, Utility.CoordStruct(1024, 0, 128), true)
	ModEnvironment.Functions.createAnim(OBJECTS.TESTANIM, Utility.CoordStruct(1024, 512, 512))

end