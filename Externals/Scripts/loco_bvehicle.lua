bvehicle_locomotor = { }

package.path = package.path .. ';../ScriptsX/?.lua'

local components = require 'components'
local Helpers = require 'Helpers'
local Placeholders = require 'Placeholders'

local comp_BVehicle = components.component:new({
	name = 'BVehicle',
	alias = 'BVehicle',

	move = Placeholders.ComponentMethod,
})

function comp_BVehicle:on_init()
	local script_type = Helpers.scriptType_TechnoRTTITable(self:container_parent())
	local phyargs = script_type:property 'physics'

	self:set_datafield('enabled', false)

	if phyargs['ntype_locomotor'] == 'bvehicle' and phyargs['vehicle_type'] == 'raycast' then
		self:set_datafield('enabled', true)
	end
end

function comp_BVehicle:on_spawn()
	local script_type = Helpers.scriptType_TechnoRTTITable(self:container_parent())
	local vehargs = script_type:property 'physics' ['raycast_vehicle_args']
	local obj = Helpers.Techno_TechnoRTTIIDTable(self:container_parent())

	if self.data['enabled'] then
		obj.Physics.vehicle = Physics.RaycastVehicle.create()
		obj.Physics.vehicle.parent = obj.Physics
		obj.Physics.vehicle:spawn()

		obj.Physics.vehicle:add_wheel(Utility.CoordStruct(38, 25, vehargs['wheel_height']), vehargs['wheel_radius'], 12)
		obj.Physics.vehicle:add_wheel(Utility.CoordStruct(38, -25, vehargs['wheel_height']), vehargs['wheel_radius'], 12)
		obj.Physics.vehicle:add_wheel(Utility.CoordStruct(-38, 25, vehargs['wheel_height']), vehargs['wheel_radius'], 24)
		obj.Physics.vehicle:add_wheel(Utility.CoordStruct(-38, -25, vehargs['wheel_height']), vehargs['wheel_radius'], 24)

		obj.Physics.vehicle:set_wheelfriction(vehargs['wheel_friction'])
		obj.Physics.vehicle:setup_wheels()
		obj.Physics.vehicle:clear_steer()
	end
end

function comp_BVehicle:on_update()

end

bvehicle_locomotor.comp_BVehicle = comp_BVehicle

return bvehicle_locomotor