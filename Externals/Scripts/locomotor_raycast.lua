local locomotor_raycast = { }

local components = require 'components'
local Placeholders = require 'Placeholders'
local H = require 'Helpers'

local comp_locoraycast = components.component:new {
	name = 'RaycastLocomotor',
	alias = 'RaycastLocomotor',

	move = Placeholders.ComponentMethod,
}

function comp_locoraycast:on_create()
	self.data['state'] = 'IDLE'
	self.data['dest_t'] = { 0, 0, 0 }
	self.data['coretp'] = Helpers.Techno_TechnoRTTIIDTable(self:container_parent())
end

function comp_locoraycast:on_update()

	local coretp = self.data['coretp']
	local scriptType = H.scriptType_TechnoRTTITable(self:container_parent())
	local phyargs = scriptType:property 'physics'
	local locoargs = phyargs['raycast_locomotor_args']

	if self:state() == 'MOVING' then
		local curpos = H.unpack_coord3(coretp:GetCoord())

		if H.vector2_distance(curpos, self:get_datafield 'dest_t') < 128 then
			self:brake_to_stop()
		else
			local curforward = H.vector2_nom({ coretp.Physics.forward_vec.x, coretp.Physics.forward_vec.y })
			local reqforward = H.vector2_nom(H.vector2_offset(self:get_datafield 'dest_t', curpos))
			local fac = 1
			if H.vector2_cross(curforward, reqforward) < 0 then fac = -1 end

			local deacc_to_turn = false
			if H.vector2_dot(curforward, reqforward) < 0.96 then
				local cen_force = phyargs['mass'] * 2.0 / 2.0
				local rot_radius = H.centri_radius(cen_force, phyargs['mass'], coretp.Physics:getVelocity())
				if rot_radius > locoargs['max_rot_radius'] then
					print(rot_radius, 'deaccing')
					deacc_to_turn = true
					self:apply_engineforce(-1 * locoargs['rotate_negativeforce'])
				end

				self:steer_safe(fac)
			else
				self:clear_steer()
			end

			if H.vector2_dot(curforward, reqforward) < 0.99 then
				self:steer_safe(fac)
			end

			if coretp.Physics.vehicle:get_current_speed() < locoargs['stablespeed'] and not deacc_to_turn then
				self:apply_engineforce(locoargs['engineforce'])
			else
				self:apply_engineforce(0)
			end

		end

	elseif self:state() == 'BREAKING_TO_STOP' then
		print 'BREAKING'
		self:brake_to_stop()

		print (coretp.Physics.vehicle:get_current_speed())
		if math.abs(coretp.Physics.vehicle:get_current_speed()) < locoargs['brake_threshold'] then
			coretp.Physics:setVelocity(0)
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
	for i = 2, 3 do
		self.data['coretp'].Physics.vehicle:brake_tyre_atonce(i, locoargs['brakingforce'])
	end
	self:clear_steer()
end

function comp_locoraycast:clear_steer()
	self.data['coretp'].Physics.vehicle:clear_steer()
end

function comp_locoraycast:steer_safe(steer)
	for i = 0, 1 do
		self.data['coretp'].Physics.vehicle:set_steer(i, steer)
	end
end

function comp_locoraycast:apply_engineforce(force)
	for i = 2, 3 do
		self.data['coretp'].Physics.vehicle:set_maxspeed(i, 100)
		self.data['coretp'].Physics.vehicle:launch_tyre(i, force)
	end
end

function comp_locoraycast:state(state)
	if state == nil then return self.data['state'] end
	self.data['state'] = state
end

function comp_locoraycast:move_to_coord_direct(vec3)
	self.data['dest_t'] = vec3
	self:state 'MOVING'
end


locomotor_raycast.comp_locoraycast = comp_locoraycast

return locomotor_raycast