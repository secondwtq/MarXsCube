components = require 'components'
bindedattr = require 'binded_attr'

composer = require 'composer'
Helpers = require 'Helpers'

function Functions.TechnoType_onLoad(self, table) end

function Functions.Abs_Techno_onUpdate(self)
	local tb = self.ExtTable

	self.elements.direction = self.Direction.degree

	tb.components:update()

end

function Functions.Abs_Techno_onSpawn(self, table)
	local scriptType = self:getTechnoType().ScriptType

	table.disabledTimer = Utility.CubeTimer(64, -1)

	table.components:spawn()
end

-- Components Container should be created in Object RTTIID table
--    it name should be 'table.components'
-- call the init(parent) method after allocing using the table(parent)
-- call init_components() method after all components are added to trigger on_init()

function Functions.Abs_Techno_onCreate(creating, table)

	-- set general properties for RTTIID table, for helpers
	table.RTTIID = creating.RTTIID
	table.TechnoType = Helpers.TechnoType_Techno(creating)
	table.Type = table.TechnoType
	table.Techno = creating
	table.AttachedToObject = creating
	table.ScriptType = Helpers.scriptType_Techno(creating)

	table.GetCoord = function (self) return creating.GetCoord(creating) end

	function table:WhatAmI () return Enums.TechnoRTTIIDTable end

	local scriptType = Helpers.scriptType_Techno(creating)

	-- alloc, init comp cont, and add components to container
	table.components = components.components_container:new()
	table.components:init(table)

	for i, v in ipairs(scriptType.components) do
		table.components:add_component(v)
	end

	table.components:init_components()

	table.isDisabled = false

	-- hack, same as table.typename, we need to add a name flag for TECHNOTYPE for debugging
	--   moreover, we need to get CubeCore Object from Object RTTIID table,
	--			and get Object RTTIID table from CubeCore Object
	--				get Type from both CubeCore Object and Object RTTIID table
	table.typename = scriptType.image

	creating.useCollSphere = scriptType.usecollsphere
	creating.rCollSphere = scriptType.rcollsphere
end
