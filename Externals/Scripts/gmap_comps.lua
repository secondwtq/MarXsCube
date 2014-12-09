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
})

function comp_GraphLineStore:init(vert1, vert2, weight)
	local edge_element = self:get_container().a['RenderElementsManger']:get_element_named('line')

	self:set_datafield('verts', { vert1+1, vert2+1 })
	self:set_datafield('weight', weight)

	edge_element.point1 = OBJ_DOTS[vert1+1]:GetCoord()
	edge_element.point2 = OBJ_DOTS[vert2+1]:GetCoord()
end

gmap_comps.comp_GraphLineStore = comp_GraphLineStore

return gmap_comps