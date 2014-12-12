gmap_locomotor = { }

package.path = package.path .. ';../ScriptsX/?.lua'

local components = require 'components'
local Helpers = require 'Helpers'
local Placeholders = require 'Placeholders'

local subcomp_LocomotorDefault = components.subcomponent:new({

	name = 'LocomotorDefault_Initial'

})

local comp_LocomotorDefault = components.component:new({
	name = 'LocomotorDefault',
	alias = 'LocomotorDefault',

	subcomponents = {
		subcomp_LocomotorDefault
	},

	move = Placeholders.ComponentMethod,
})

function comp_LocomotorDefault:on_init()
	self:set_datafield('state', 'NONE')

	local scriptType = Helpers.scriptType_TechnoRTTITable(self:container_parent())
	local phyargs = scriptType:property 'physics'
	if phyargs['ntype_locomotor'] == 'default' then

		self:set_datafield('state', 'IDLE') -- state, NONE, IDLE, BRAKING, MOVING
		self:set_datafield('dest_vector3', {0, 0, 0}) -- current dest coord, a part of path
		self:set_datafield('dest_vecarray', { })
		self:set_datafield('current_pathnode', 1)

	end
end

function comp_LocomotorDefault:state(state)
	print('comp_LocomotorDefault: ', 'entering state ', state)
	self:set_datafield('state', state)
end

function comp_LocomotorDefault:in_state(state)
	return self:get_datafield 'state' == state
end

function comp_LocomotorDefault:on_update()
	if not self:in_state 'IDLE' then
		local obj = Helpers.Techno_TechnoRTTIIDTable(self:container_parent())
		local loco_args = Helpers.scriptType_TechnoRTTITable(self:container_parent()):property 'physics'['nlocomotor_args']

		local current_forward_vel = obj.Physics:getVelocity()
		local current_pos = Helpers.unpack_coord3(obj:GetCoord())

		-- print('velocity', current_forward_vel)

		if self:in_state 'MOVING' then

			-- if too near to final dest, then start braking
			if Helpers.vector3_distance(current_pos, self:get_datafield 'dest_vector3') < 128 then
				-- all movement is treated as a waypointed navagation, define path with a vector3 array
				if self:path_ended() then
					print("current pos", unpack(current_pos))
					self:state 'BRAKING'
				else
					self:advance_path()
				end
			else
				local current_rotation = obj.Physics:getMainRotation()
				local current_deg = math.deg(current_rotation)
				local required_rotation = Helpers.rad_from_vector2(Helpers.vector2_offset(self:get_datafield 'dest_vector3', current_pos))
				local required_deg = math.deg(required_rotation)
				local required_distance = Helpers.vector2_len(Helpers.vector2_offset(self:get_datafield 'dest_vector3', current_pos))

				if current_forward_vel >= 1 then

					local factor = 1
					local vector2_offset = Helpers.local_vector2(current_pos, Helpers.unpack_coord2(obj.Physics:getLinearVelocity()), self:get_datafield 'dest_vector3')
					if vector2_offset[1] > 0 then factor = -1*factor end

					if math.abs(current_forward_vel) >= loco_args['stablespeed'] / 24 then
						if Helpers.absoffset_rad(required_rotation, current_rotation) > 0.15 then
							obj.Physics:applyCentralForce_Vertical(factor * loco_args['rot'])
							-- print('rotating ', 'velocity', current_forward_vel, 'radius', Helpers.centri_radius(loco_args['rot'], Helpers.scriptType_TechnoRTTITable(self:container_parent()):property 'physics'['mass'], current_forward_vel))

							local rotate_radius = Helpers.centri_radius(loco_args['rot'], Helpers.scriptType_TechnoRTTITable(self:container_parent()):property 'physics'['mass'], current_forward_vel)

							-- brake, when rotating
							if rotate_radius > required_distance/4 then
								obj.Physics:applyCentralForce_Directional(-2*loco_args['rotate_negativeforce'])
							end
						end
					end
					obj.Physics:setDirectionTo(math.deg(Helpers.rad_from_vector2(Helpers.unpack_coord2(obj.Physics:getLinearVelocity()))))
				end

				-- if speed is too low, and is not rotating, then apply engine force
				if current_forward_vel < loco_args['stablespeed'] then
					if not (current_forward_vel > loco_args['stablespeed']/4 and Helpers.absoffset_rad(required_rotation, current_rotation) > 0.3) then
						obj.Physics:applyCentralForce_Directional(loco_args['engineforce'])
					end
				end
			end

		elseif self:in_state 'BRAKING' then

			-- if object is not slow enough, apply a negative force
			if math.abs(current_forward_vel) > loco_args['stablespeed'] / 5 then
				obj.Physics:applyCentralForce_Directional(-1*Helpers.sign(current_forward_vel)*loco_args['brakingforce'])
			else -- or stop it, immediately
				obj.Physics:setVelocity(0)
				obj.Physics:setMainRotationVelocity(0)
				print("current pos", unpack(current_pos))
				self:state 'IDLE'
			end

		end
	end

end

-- directed move is delegated to path move
function comp_LocomotorDefault:move_to_coord_direct(vector3) self:move_path({vector3}) end

function comp_LocomotorDefault:move_path(array_path)
	self:set_datafield('dest_vecarray', array_path)
	self:set_datafield('current_pathnode', 1)
	self:set_datafield('dest_vector3', self:get_datafield 'dest_vecarray' [1])
	self:set_datafield('state', 'MOVING')
	Helpers.Techno_TechnoRTTIIDTable(self:container_parent()).Physics:activate()
end

-- move to next path node, used by on_update()
function comp_LocomotorDefault:advance_path()
	local current_idx = self:get_datafield 'current_pathnode'
	local current_node = self:get_datafield 'dest_vecarray' [current_idx+1]
	self:set_datafield('current_pathnode', current_idx+1)
	self:set_datafield('dest_vector3', current_node)
end

-- judge whether current path is finished, used by on_update()
function comp_LocomotorDefault:path_ended()
	local current_idx = self:get_datafield 'current_pathnode'
	return current_idx >= #(self:get_datafield 'dest_vecarray')
end

gmap_locomotor.comp_LocomotorDefault = comp_LocomotorDefault

return gmap_locomotor