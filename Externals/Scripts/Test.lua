gmap_dots = require('gmap_dots')
gmap_edges = require('gmap_edges')
Helpers = require('Helpers')

DATA_DOTS = gmap_dots.data
DATA_EDGES = gmap_edges.data
OBJ_DOTS = { }
OBJ_EDGES = { }

function Functions.TestManger_onTestInit()
	Objects.Map.GetInstance():CreateEmptyMap(60, 50)
	ModEnvironment.Functions.createAnim(OBJECTS.TESTANIM, Utility.CoordStruct(1024, 512, 512))

	local data_dots_new = { }
	for i, dot in pairs(DATA_DOTS) do
		local t = { -1*(dot[1] * 192 - 64 * 2), dot[2] * 192 + 64 * 28 }
		Helpers.tblinsert(data_dots_new, t)
	end
	DATA_DOTS = data_dots_new

	for i, dot in ipairs(DATA_DOTS) do
		local dot_techno = ModEnvironment.Functions.createTechno(OBJECTS.GRAPH_NODE, Utility.CoordStruct(dot[2], dot[1], 0)).ExtTable
		OBJ_DOTS[i] = dot_techno
	end

	for i, edge in ipairs(DATA_EDGES) do
		-- we really need some functions to fetch the corners of map
		local edge_techno = ModEnvironment.Functions.createTechno(OBJECTS.GRAPH_LINE, Utility.CoordStruct(0, -25*64, 0)).ExtTable

		local GLS = edge_techno.components.a['GraphLineStore']
		GLS:init(unpack(edge))

		OBJ_EDGES[i] = edge_techno
	end

end