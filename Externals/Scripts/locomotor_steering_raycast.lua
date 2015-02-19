local components = require 'components'
local Placeholders = require 'Placeholders'
local H = require 'Helpers'
local v = require 'Vhere'
local object = require 'object'
local steering = require 'steering'

local comp_LocomotorSteer_Raycast = components.component:new {
	name = 'SteeringLocomotor_Raycast',
	alias = 'SteeringLocomotor_Raycast'
}

local vehicle_wrapper = object.object:new {
	parent = nil
}

function vehicle_wrapper:position()
	return v.coord2vec2(self.parent:GetCoord()) end

function vehicle_wrapper:velocity()
	return v.coord2vec2(self.parent.Physics:getLinearVelocity()) end

function vehicle_wrapper:speed()
	return self.parent.Physics:getVelocity() * 10 end

function comp_LocomotorSteer_Raycast:move_path(vec3_array, pathway)
	self.data.pathway = pathway
	self:state 'MOVING'
end

function comp_LocomotorSteer_Raycast:on_create()
	self.data.core = H.Techno_TechnoRTTIIDTable(self:container_parent())
	self.data.vehicle = vehicle_wrapper:new { parent = self.data.core }
	self.data.steering = steering.steering:new { parent = self.data.vehicle }
	self.data.state = 'IDLE'
	self.data.pathway = nil
end

function comp_LocomotorSteer_Raycast:on_init()
	self.data.com_vehicle = self:get_container().a['BVehicle']
end

function comp_LocomotorSteer_Raycast:on_update()
	local core = self.data['core']
	local phyargs = H.scriptType_TechnoRTTITable(self:container_parent()):property 'physics'
	local locoargs = phyargs['raycast_locomotor_args']

	if self:state() == 'MOVING' then
		local force = v.vector2d(0, 0)
		local force_path = self.data.steering:steer_for_path(self.data.pathway)

		force = force + force_path
		force = force / 10

		if force:len() > locoargs['stablespeed'] then
			force = force:nom() * locoargs['stablespeed']
		end

		steering.vel_desired = force * 10

		force = force * phyargs['mass']

		self:apply_force(force)
	end
end

function abs_clamp(src, max_value)
	if src > 0 and src > max_value then src = max_value end
	if src < 0 and src < -max_value then src = -max_value end
	return src
end

function sign(src)
	if src > 0 then return 1 elseif src < 0 then return -1 end
	return 0
end

function comp_LocomotorSteer_Raycast:apply_force(force)
	local core = self.data['core']
	local phyargs = H.scriptType_TechnoRTTITable(self:container_parent()):property 'physics'
	local locoargs = phyargs['raycast_locomotor_args']

	local curforward = v.coord2vec2(core.Physics.forward_vec):nom()

	-- print(core.Physics:getVelocity())

	local engineforce = v.dot(force, curforward)
	engineforce = abs_clamp(engineforce, locoargs['engineforce'])
	if math.abs(core.Physics:getVelocity()) > locoargs['stablespeed'] and engineforce > 0 then engineforce = 0 end
	self:apply_engineforce(engineforce)

	local steer_force = v.dot(force, v.vector2d(-curforward.y, curforward.x))
	-- print('steer_force', steer_force)
	core.Physics:applyCentralForce_Vertical(steer_force)
	self:steer_safe(sign(steer_force))
end

function comp_LocomotorSteer_Raycast:clear_steer()
	self.data['core'].Physics.vehicle:clear_steer()
end

function comp_LocomotorSteer_Raycast:steer_safe(steer)
	for t, i in ipairs(self.data.com_vehicle.data.wheels_steer) do
		self.data['core'].Physics.vehicle:set_steer(i, steer)
	end
end

function comp_LocomotorSteer_Raycast:apply_engineforce(force)
	for t, i in ipairs(self.data.com_vehicle.data.wheels_engine) do
		self.data['core'].Physics.vehicle:set_maxspeed(i, 100)
		self.data['core'].Physics.vehicle:launch_tyre(i, force)
	end
end

function comp_LocomotorSteer_Raycast:state(state)
	if state == nil then return self.data['state'] end
	self.data['state'] = state
end

return { comp_LocomotorSteer_Raycast = comp_LocomotorSteer_Raycast }