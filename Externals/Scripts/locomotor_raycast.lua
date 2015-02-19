local locomotor_raycast = { }

local components = require 'components'
local Placeholders = require 'Placeholders'
local H = require 'Helpers'
local vhere = require 'Vhere'

local comp_locoraycast = components.component:new {
	name = 'RaycastLocomotor',
	alias = 'RaycastLocomotor',

	move = Placeholders.ComponentMethod,
}

function comp_locoraycast:move_to_coord_direct(vec3)
	self.data['dest_t'] = vec3
	self.data.is_path = false
	self:state 'MOVING'
end

function comp_locoraycast:move_path(vec3_array, pathway)
	self.data['path'] = vec3_array
	self.data['current_node'] = 1
	self.data['dest_t'] = vec3_array[1]

	self.data['pathway'] = pathway

	self.data.is_path = true
	self:state 'MOVING'
end

function comp_locoraycast:on_create()
	self.data['state'] = 'IDLE'
	self.data['is_path'] = false
	self.data['dest_t'] = { 0, 0, 0 }
	self.data['core'] = H.Techno_TechnoRTTIIDTable(self:container_parent())

	self.data['path'] = { }
	self.data['current_node'] = 1

	self.data['pathway'] = nil

	self.data['is_returning'] = false
end

function comp_locoraycast:on_init()
	self.data.com_vehicle = self:get_container().a['BVehicle']
end

function comp_locoraycast:on_update()

	local core = self.data['core']
	local phyargs = H.scriptType_TechnoRTTITable(self:container_parent()):property 'physics'
	local locoargs = phyargs['raycast_locomotor_args']

	if self:state() == 'MOVING' then
		-- print(string.format('%.2f', core.Physics:getVelocity()))

		local curpos = vhere.coord2vec2(core:GetCoord())
		local curforward = vhere.coord2vec2(core.Physics.forward_vec):nom()

		print(self.data.pathway:mapPointToPathDistance(vhere.vec2glm3(curpos)))

		local look_ahead_offline = vhere.vector2d(curforward.y, -curforward.x)
		local look_ahead_pos1, look_ahead_pos2 = curpos + look_ahead_offline * locoargs['lookahead_offset'],
												curpos - look_ahead_offline * locoargs['lookahead_offset']
		local look_ahead_end1 = look_ahead_pos1 + curforward * 256
		local look_ahead_end2 = look_ahead_pos2 + curforward * 256
		local cla_start1, cla_end1, cla_start2, cla_end2 = 
				vhere.vec2coord(look_ahead_pos1, 24), vhere.vec2coord(look_ahead_end1, 64), vhere.vec2coord(look_ahead_pos2, 24), vhere.vec2coord(look_ahead_end2, 64)

		Bullet.DebugDrawer.draw_line(cla_start1, cla_end1)
		Bullet.DebugDrawer.draw_line(cla_start2, cla_end2)
		local rt1, rt2 = Physics.createRayTestTechnoOnly(cla_start1, cla_end1), Physics.createRayTestTechnoOnly(cla_start2, cla_end2)
		rt1:set_except(core) rt2:set_except(core)
		rt1:perform() rt2:perform()

		local need_steering = rt1:hit() or rt2:hit()
		if need_steering then
			local hit_point
			if rt1:hit() then hit_point = vhere.coord2vec2(rt1:hit_point())
			else hit_point = vhere.coord2vec2(rt2:hit_point()) end

			local avoid = curpos - hit_point
			local avoid_refl = curforward:reflect(avoid) * 384
			Bullet.DebugDrawer.draw_line(core:GetCoord(), vhere.vec2coord(curpos+avoid_refl))
			self.data['dest_org'] = self.data.path[self.data.current_node]
			self.data['dest_t'] = curpos + avoid_refl

			Bullet.DebugDrawer.draw_line(core:GetCoord(), vhere.vec2coord(hit_point))
			print('steering')

			if H.vector2_distance(curpos, self.data.dest_org) < 128 then
				if self:path_ended() then
					self:brake_to_stop()
					self.data.is_returning = false
				else
					self:advance_path()
				end
			end

		else
			self.data.is_returning = false
			if not self:path_ended() then self:renter_path() end
		end

		if H.vector2_distance(curpos, self.data['dest_t']) < 128 then
			if not need_steering then
				if not self:path_ended() then
					self:advance_path()
					self.data.is_returning = false
				else
					self:brake_to_stop()
					self.data.is_returning = false
				end
			else
				self:renter_path()
				self.data.is_returning = false
			end
		else
			local reqforward = H.vector2_nom(H.vector2_offset(self:get_datafield 'dest_t', curpos))
			local fac = 1
			if H.vector2_cross(curforward, reqforward) < 0 then fac = -1 end
			if self.data.is_returning then fac = fac * -1 end

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

			if need_steering then
				core.Physics:applyCentralForce_Vertical(fac * locoargs['extra_rotforce'])
				self:apply_engineforce(-1 * locoargs['rotate_negativeforce'])
			end

			if not self.data.is_returning then
				if math.abs(core.Physics.vehicle:get_current_speed()) < locoargs['stablespeed'] and not deacc_to_turn then
					self.data['core'].Physics.vehicle:clear_brake()
					self:apply_engineforce(locoargs['engineforce'])
				else self:apply_engineforce(0) end
			elseif core.Physics:getVelocity() > -1 * locoargs['stablespeed'] then
				self:apply_engineforce(-1 * locoargs['engineforce'])
			end

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

function comp_locoraycast:path_ended()
	local current_idx = self.data.current_node
	return current_idx > #(self.data.path)
end

function comp_locoraycast:advance_path()
	local curidx = self.data.current_node
	self.data.current_node = curidx+1
	self.data.dest_t = self.data.path[curidx]
end

function comp_locoraycast:renter_path()
	self.data.dest_t = self.data.path[self.data.current_node]
end

function comp_locoraycast:state(state)
	if state == nil then return self.data['state'] end
	self.data['state'] = state
end

locomotor_raycast.comp_locoraycast = comp_locoraycast

return locomotor_raycast