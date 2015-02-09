Utility.DoImport("mapdef.lua")

gmap_dots = require 'gmap_dots'
gmap_edges = require 'gmap_edges'
Helpers = require 'Helpers'

-- global variables
DATA_DOTS = gmap_dots.data
DATA_EDGES = gmap_edges.data
OBJ_DOTS = { }
OBJ_EDGES = { }

GRAPH_GLOBAL = None

TECHNO_SELECTED = nil

function grit_test()

	local start = Utility.CubePoint(0, 0)
	local eend = Utility.CubePoint(1024, 0)
	print(Utility.GetTime_Clock())
	local vec = Grit.instance():find_path(start, eend)
	print(Utility.GetTime_Clock())
	print(vec:size())
	print(vec:at(0).x, vec:at(0).y)
	print(vec:at(1).x, vec:at(1).y)
	print(vec:at(2).x, vec:at(2).y)
	print(vec:at(3).x, vec:at(3).y)
	print(vec:at(4).x, vec:at(4).y)

end

function acheroncube_test_callback()
	print('path got', Utility.GetTime_Clock())
end

function acheroncube_test()

	local start = Utility.CubePoint(0, 0)
	local eend = Utility.CubePoint(1024, 0)
	print(Utility.GetTime_Clock())
	local vec = Acheron.find_path_async(start, eend, acheroncube_test_callback)
	print(Utility.GetTime_Clock())

end

-- run when game started
function Functions.TestManger_onTestInit()

	local vec0 = Utility.CubePoint(256, 256)
	local vec1 = Utility.CubePoint(512, 512)
	local vec2 = Utility.CubePoint(768, 256)
	local vec3 = Utility.CubePoint(768, -256)
	local vec4 = Utility.CubePoint(512, -768)
	local vec5 = Utility.CubePoint(256, -256)
	local obs = Grit.GritObstacle.create()
	obs:pts():push_back(vec0)
	obs:pts():push_back(vec1)
	obs:pts():push_back(vec2)
	obs:pts():push_back(vec3)
	obs:pts():push_back(vec4)
	obs:pts():push_back(vec5)
	Grit.instance():add_obs(obs)

	Objects.Map.GetInstance():CreateEmptyMap(60, 50) -- create map
	GRAPH_GLOBAL = Appins.Gmap.Graph(#DATA_DOTS) -- create graph data structure

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
	end

	-- transform the coord of original data for display in map
	--		we just abandon original table
	DATA_DOTS = transform_dots(DATA_DOTS)

	-- local background = ModEnvironment.Functions.createTechno(OBJECTS.SATELITE_BG, Utility.CoordStruct(64*27+56, -36, 0))
	-- background.temp_ZOffset = true
	local cycle = ModEnvironment.Functions.createTechno(OBJECTS.TESTTECHNO, Utility.CoordStruct(256, 0, 64), true)
	local rail = ModEnvironment.Functions.createTechno(OBJECTS.TESTTECHNO_PHY, Utility.CoordStruct(256, 256, 128), true)
	local rail2 = ModEnvironment.Functions.createTechno(OBJECTS.TESTTECHNO_PHY, Utility.CoordStruct(1024, 0, 128), true)
	ModEnvironment.Functions.createAnim(OBJECTS.TESTANIM, Utility.CoordStruct(1024, 512, 512))

	-- create GameObjects for nodes, and fill OBJ_DOTS
	for i, dot in ipairs(DATA_DOTS) do
		local dot_techno = ModEnvironment.Functions.createTechno(OBJECTS.GRAPH_NODE, Utility.CoordStruct(dot[2], dot[1], 0)).ExtTable
		dot_techno.components.a['GraphNodeStore']:init(i-1, {dot[2], dot[1], 0})
		OBJ_DOTS[i] = dot_techno
	end

end

function transform_dots(src)
	local data_dots_new = { }
	for i, dot in pairs(src) do
		-- local t = { -1*(dot[1] * 192 - 64 * 2), dot[2] * 192 + 64 * 28 }
		local t = { -1*(dot[1] * 176 - 64 * 2), dot[2] * 128 + 64 * 28 }

		table.insert(data_dots_new, #data_dots_new+1, t)
	end
	return data_dots_new
end

-- find the nearest node (RTTITable) of a CoordStruct from OBJ_DOTS
-- coord must be a CoordStruct, max_dist can be empty
function find_nearest_node(coord, max_dist)
	local current_node = OBJ_DOTS[1]
	coord.z = 0
	local current_dis = Helpers.coord_distance(coord, OBJ_DOTS[1].GetCoord())

	for i, nodeobj in ipairs(OBJ_DOTS) do
		local dis_new = Helpers.coord_distance(coord, nodeobj.GetCoord())
		if dis_new < current_dis then
			current_dis = dis_new
			current_node = nodeobj
		end
	end

	-- max_dist as a threshold
	if max_dist ~= nil and current_dis > max_dist then return nil end
	return current_node
end

-- calculate nearest path of an object's current position to a specificed node
--- args: technot - a Techno RTTITable, target_node a Node's RTTITable
function move_techno_graph(technot, target_node)
	-- init bellman - ford object
	local bf_shortest = Appins.Gmap.bellman_ford_shortest(GRAPH_GLOBAL, technot.components.a['GraphVehicle']:get_datafield 'current_node')
	bf_shortest:go() -- execute the algorithm
	local path_nodes = Appins.Gmap.bellman_ford_shortest.extract_path(bf_shortest, target_node.components.a['GraphNodeStore']:get_datafield 'idx_initial')

	-- convert array of node index to array of vector3
	local path_nodes_vec3 = { }
	for i, node in ipairs(path_nodes) do
		local coord = OBJ_DOTS[node+1].components.a['GraphNodeStore']:get_datafield 'vec3_coord'
		Helpers.tblinsert(path_nodes_vec3, coord)
	end

	local script_type = Helpers.scriptType_TechnoRTTITable(technot)
	if script_type:property 'physics' ['use_vehicle'] == true then
		Helpers.Techno_TechnoRTTIIDTable(technot).Physics.vehicle:launch()
	else
		-- move object
		technot.components.a['LocomotorDefault']:move_path(path_nodes_vec3)	
	end

end