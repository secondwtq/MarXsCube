bvehicle_locomotor = { }

package.path = package.path .. ';../ScriptsX/?.lua'

local components = require 'components'
local Helpers = require 'Helpers'
local Placeholders = require 'Placeholders'

local subcomp_BVehicle = components.subcomponent:new({
	name = 'BVehicle_Initial'
})

local comp_BVehicle = components.component:new({
	name = 'BVehicle',
	alias = 'BVehicle',

	subcomponents = { subcomp_BVehicle },

	move = Placeholders.ComponentMethod,
})

function comp_BVehicle:on_init()
	local script_type = Helpers.scriptType_TechnoRTTITable(self:container_parent())
	local phyargs = script_type:property 'physics'

	self:set_datafield('enabled', false)

	if phyargs['ntype_locomotor'] == 'bvehicle' then
		self:set_datafield('enabled', true)
	end
end

function comp_BVehicle:on_spawn()
	local script_type = Helpers.scriptType_TechnoRTTITable(self:container_parent())
	local phyargs = script_type:property 'physics'
	local obj = Helpers.Techno_TechnoRTTIIDTable(self:container_parent())

	if phyargs['use_vehicle'] then
		obj.Physics.vehicle = Physics.BulletVehicle.create()
		obj.Physics.vehicle.parent = obj.Physics
		obj.Physics.vehicle:spawn()

		local wheel_radius = 7
		local wheel_height = -0
		obj.Physics.vehicle:add_wheel(Utility.CoordStruct(48, 18, wheel_height), wheel_radius)
		obj.Physics.vehicle:add_wheel(Utility.CoordStruct(-48, 18, wheel_height), wheel_radius)
		obj.Physics.vehicle:add_wheel(Utility.CoordStruct(48, -18, wheel_height), wheel_radius)
		obj.Physics.vehicle:add_wheel(Utility.CoordStruct(-48, -18, wheel_height), wheel_radius)

		obj.Physics.vehicle:setup_wheels()
		-- obj.Physics.vehicle:launch()
	end
end

function comp_BVehicle:on_update()
	if self:get_datafield 'enabled' then
		local obj = Helpers.Techno_TechnoRTTIIDTable(self:container_parent())
		obj.Physics.vehicle:clear_steer()
	end
end

bvehicle_locomotor.comp_BVehicle = comp_BVehicle

return bvehicle_locomotor