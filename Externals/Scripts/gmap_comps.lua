gmap_comps = { }

package.path = package.path .. ';../ScriptsX/?.lua'

local components = require 'components'
local Helpers = require 'Helpers'
local Placeholders = require 'Placeholders'

local subcomp_GraphLineStore = components.subcomponent:new({
	name = "GraphLineStore_Initial",
})

local comp_GraphLineStore = components.component:new({
	name = "GraphLineStore",
	alias = "GraphLineStore",

	subcomponents = {
		subcomp_GraphLineStore	
	},

	init = Placeholders.ComponentMethod,
	connect = Placeholders.ComponentMethod
})

function comp_GraphLineStore:init(vert1, vert2, weight)
	local edge_element = self:get_container().a['RenderElementsManger']:get_element_named('line')
	edge_element.point1 = OBJ_DOTS[vert1+1]:GetCoord()
	edge_element.point2 = OBJ_DOTS[vert2+1]:GetCoord()

	self:set_datafield('verts_lua', { vert1+1, vert2+1 })
	self:set_datafield('verts_initial', { vert1, vert2 })
	self:set_datafield('weight', weight)
end

function comp_GraphLineStore:connect(vert1, vert2)
	local verts = self:get_datafield 'verts_initial'
	return ((vert1 == verts[1] and vert2 == verts[2]) or (vert1 == verts[2] and vert2 == verts[1]))
end

gmap_comps.comp_GraphLineStore = comp_GraphLineStore

local subcomp_GraphNodeStore = components.subcomponent:new({
	name = "GraphNodeStore_Initial"
})

local comp_GraphNodeStore = components.component:new({
	name = "GraphNodeStore",
	alias = "GraphNodeStore",

	subcomponents = {
		subcomp_GraphNodeStore
	},

	init = Placeholders.ComponentMethod,
})

function comp_GraphNodeStore:init(idx_initial, vec3_coord)
	self:set_datafield('idx_initial', idx_initial)
	self:set_datafield('vec3_coord', vec3_coord)
end

gmap_comps.comp_GraphNodeStore = comp_GraphNodeStore

local subcomp_GraphVehicle = components.subcomponent:new({
	name = "GraphVehicle_Initial"
})

local comp_GraphVehicle = components.component:new({
	name = "GraphVehicle",
	alias = "GraphVehicle",

	subcomponents = { subcomp_GraphVehicle }
})

function comp_GraphVehicle:on_init()
	self:set_datafield('current_node', 0)
end

function comp_GraphVehicle:on_update()
	-- update current node datafield
	self:set_datafield('current_node', find_nearest_node(self:container_parent():GetCoord()).components.a['GraphNodeStore']:get_datafield 'idx_initial')
end

gmap_comps.comp_GraphVehicle = comp_GraphVehicle

return gmap_comps