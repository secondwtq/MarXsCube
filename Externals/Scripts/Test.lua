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

-- run when game started
function Functions.TestManger_onTestInit()
	Objects.Map.GetInstance():CreateEmptyMap(60, 50) -- create map
	GRAPH_GLOBAL = Appins.Gmap.Graph(#DATA_DOTS) -- create graph data structure

	-- transform the coord of original data for display in map
	--		we just abandon original table
	DATA_DOTS = transform_dots(DATA_DOTS)

	local cycle = ModEnvironment.Functions.createTechno(OBJECTS.TESTTECHNO, Utility.CoordStruct(0, 0, 0), true)
	local rail = ModEnvironment.Functions.createTechno(OBJECTS.TESTTECHNO_PHY, Utility.CoordStruct(128, 0, 0), true)
	-- ModEnvironment.Functions.createAnim(OBJECTS.TESTANIM, Utility.CoordStruct(1024, 512, 512))

	-- create GameObjects for nodes, and fill OBJ_DOTS
	for i, dot in ipairs(DATA_DOTS) do
		local dot_techno = ModEnvironment.Functions.createTechno(OBJECTS.GRAPH_NODE, Utility.CoordStruct(dot[2], dot[1], 0)).ExtTable
		dot_techno.components.a['GraphNodeStore']:init(i-1, {dot[2], dot[1], 0})
		OBJ_DOTS[i] = dot_techno
	end

	-- create GameObjects for edges, and fill OBJ_EDGES
	for i, edge in ipairs(DATA_EDGES) do
		-- we really need some functions to fetch the corners of map
		local edge_techno = ModEnvironment.Functions.createTechno(OBJECTS.GRAPH_LINE, Utility.CoordStruct(0, -25*64, 0)).ExtTable
		edge_techno.components.a['GraphLineStore']:init(unpack(edge))
		OBJ_EDGES[i] = edge_techno

		GRAPH_GLOBAL:connect(unpack(edge))
	end

	move_techno_graph(cycle.ExtTable, OBJ_DOTS[9+1])
	move_techno_graph(cycle.ExtTable, OBJ_DOTS[19+1])

end

function transform_dots(src)
	local data_dots_new = { }
	for i, dot in pairs(src) do
		local t = { -1*(dot[1] * 192 - 64 * 2), dot[2] * 192 + 64 * 28 }
		Helpers.tblinsert(data_dots_new, t)
	end
	return data_dots_new
end

-- find the nearest node (RTTITable) of a CoordStruct from OBJ_DOTS
-- coord must be a CoordStruct, max_dist can be empty
function find_nearest_node(coord, max_dist)
	local current_node = OBJ_DOTS[1]
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

	-- move object
	technot.components.a['LocomotorDefault']:move_path(path_nodes_vec3)
end