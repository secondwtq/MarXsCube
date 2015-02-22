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
	parent = nil }

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
	local curpos = v.coord2vec2(core:GetCoord())

	if self:state() == 'MOVING' then
		local force = v.vector2d(0, 0)
		local force_path = self.data.steering:steer_for_path(self.data.pathway)
		local force_avoid = self.data.steering:steer_to_avoid()
		Bullet.DebugDrawer.draw_line(v.vec2coord(curpos), v.vec2coord(curpos+force_path))
		Bullet.DebugDrawer.draw_line(v.vec2coord(curpos), v.vec2coord(curpos+force_avoid))

		force = force + force_path
		force = force + force_avoid
		self.data.steering.vel_desired = force
		force = force / 10

		if force:len() > locoargs['stablespeed'] then
			force = force:nom() * locoargs['stablespeed'] end

		self:adjust_velocity(force)
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

function comp_LocomotorSteer_Raycast:adjust_velocity(desired)
	local core, phyargs = self.data['core'], H.scriptType_TechnoRTTITable(self:container_parent()):property 'physics'
	local locoargs = phyargs['raycast_locomotor_args']

	local curforward, curforward_speed = v.coord2vec2(core.Physics.forward_vec), core.Physics:getVelocity()

	local forward_speed = abs_clamp(v.dot(desired, curforward), locoargs['stablespeed'])

	local engineforce = locoargs['engineforce']
	if math.abs(curforward_speed) > locoargs['stablespeed'] and engineforce > 0 then engineforce = 0 end

	local steer_force = v.dot(desired:nom(), v.vector2d(-curforward.y, curforward.x):nom())
	local sim = v.dot(desired:nom(), curforward)
	local cen_force = phyargs['mass']
	local rot_radius = H.centri_radius(cen_force, phyargs['mass'], curforward_speed) * 10
	local fac = sign(steer_force)
	if locoargs['use_aux_rot'] and rot_radius > locoargs['max_rot_radius'] then
		local rot_force = fac * phyargs['mass'] * 2
		core.Physics:applyImpulse_Vertical(rot_force/5, Utility.Float3D(48, 0, 0))
	end

	core.Physics.vehicle:clear_brake()

	if sim < 0.96 then
		self:steer_safe(fac)

		if locoargs['brake_when_turning'] and rot_radius > locoargs['max_rot_radius'] then
			if sim < 0.91 then
				if engineforce > 0 then engineforce = 0 end
				self:brake_to_deacc()
			end
		end

	else
		self:clear_steer()
	end

	self:apply_engineforce(engineforce)
	-- print(string.format("(%.2f, %.2f) %.2f %.2f", curforward.x, curforward.y, sim, rot_radius))
end

function comp_LocomotorSteer_Raycast:clear_steer()
	self.data['core'].Physics.vehicle:clear_steer()
end

function comp_LocomotorSteer_Raycast:brake_to_deacc()
	local scriptType = H.scriptType_TechnoRTTITable(self:container_parent())
	local locoargs = scriptType:property 'physics' ['raycast_locomotor_args']
	for t, i in ipairs(self.data.com_vehicle.data.wheels_brake) do
		self.data['core'].Physics.vehicle:brake_tyre_atonce(i, locoargs['brakingforce'])
	end
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