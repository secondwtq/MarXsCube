local locomotor_raycast = { }

local components = require 'components'
local Placeholders = require 'Placeholders'
local H = require 'Helpers'

local comp_locoraycast = components.component:new {
	name = 'RaycastLocomotor',
	alias = 'RaycastLocomotor',

	move = Placeholders.ComponentMethod,
}

function comp_locoraycast:move_to_coord_direct(vec3)
	self.data['dest_t'] = vec3
	self:state 'MOVING'
end

function comp_locoraycast:on_create()
	self.data['state'] = 'IDLE'
	self.data['dest_t'] = { 0, 0, 0 }
	self.data['core'] = Helpers.Techno_TechnoRTTIIDTable(self:container_parent())
end

function comp_locoraycast:on_init()
	self.data.com_vehicle = self:get_container().a['BVehicle']
end

function comp_locoraycast:on_update()

	local core = self.data['core']
	local phyargs = H.scriptType_TechnoRTTITable(self:container_parent()):property 'physics'
	local locoargs = phyargs['raycast_locomotor_args']

	if self:state() == 'MOVING' then
		local curpos = H.unpack_coord3(core:GetCoord())

		if H.vector2_distance(curpos, self:get_datafield 'dest_t') < 128 then
			self:brake_to_stop()
		else
			local curforward, reqforward = H.vector2_nom({ core.Physics.forward_vec.x, core.Physics.forward_vec.y }),
										H.vector2_nom(H.vector2_offset(self:get_datafield 'dest_t', curpos))
			local fac = 1
			if H.vector2_cross(curforward, reqforward) < 0 then fac = -1 end

			local deacc_to_turn, sim
			deacc_to_turn, sim = false, H.vector2_dot(curforward, reqforward)
			if sim < 0.97 then
				local cen_force = phyargs['mass'] * 2.0 / 2.0
				local rot_radius = H.centri_radius(cen_force, phyargs['mass'], core.Physics:getVelocity())
				if rot_radius > locoargs['max_rot_radius'] then
					deacc_to_turn = true
					self:apply_engineforce(-1 * locoargs['rotate_negativeforce'])

					if sim < 0.86 then
						core.Physics:applyCentralForce_Vertical(fac * locoargs['extra_rotforce'])
					end
				end
			end

			if sim < 0.99 then self:steer_safe(fac) else self:clear_steer() end

			if core.Physics.vehicle:get_current_speed() < locoargs['stablespeed'] and not deacc_to_turn then
				self:apply_engineforce(locoargs['engineforce'])
			else self:apply_engineforce(0) end

		end

	elseif self:state() == 'BREAKING_TO_STOP' then
		self:brake_to_stop()
		local abspeed = math.abs(core.Physics.vehicle:get_current_speed())
		if abspeed < locoargs['brake_extrathreshold'] then
			core.Physics:applyCentralForce_Directional(-locoargs['extra_brakeforce'])
		end
		if abspeed < locoargs['brake_threshold'] then
			core.Physics:setVelocity(0)
			self:state 'IDLE'
		end
	end
end

function comp_locoraycast:stop_final()

end

function comp_locoraycast:brake_to_stop()
	local scriptType = H.scriptType_TechnoRTTITable(self:container_parent())
	local locoargs = scriptType:property 'physics' ['raycast_locomotor_args']
	self:state 'BREAKING_TO_STOP'
	for t, i in ipairs(self.data.com_vehicle.data.wheels_brake) do
		self.data['core'].Physics.vehicle:brake_tyre_atonce(i, locoargs['brakingforce'])
	end
	self:clear_steer()
end

function comp_locoraycast:clear_steer()
	self.data['core'].Physics.vehicle:clear_steer()
end

function comp_locoraycast:steer_safe(steer)
	for t, i in ipairs(self.data.com_vehicle.data.wheels_steer) do
		self.data['core'].Physics.vehicle:set_steer(i, steer)
	end
end

function comp_locoraycast:apply_engineforce(force)
	for t, i in ipairs(self.data.com_vehicle.data.wheels_engine) do
		self.data['core'].Physics.vehicle:set_maxspeed(i, 100)
		self.data['core'].Physics.vehicle:launch_tyre(i, force)
	end
end

function comp_locoraycast:state(state)
	if state == nil then return self.data['state'] end
	self.data['state'] = state
end

locomotor_raycast.comp_locoraycast = comp_locoraycast

return locomotor_raycast