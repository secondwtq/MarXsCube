-- this file is part of Project MarXsCube Prototype.
--
--	use in other modules:
--
--		InputHandler = require 'InputHandler'
--
--	http://github.com/secondwtq/MarXsCube

local InputHandler = { }

local Helpers = require 'Helpers'

-- called when mouse press on an object
-- args: mouse_status: same as Functions.Session_MousePress
--		ray: the RayTestSingle object used to test ray collision
function InputHandler.MousePress_OnObject(mouse_status, ray)
	local hit_point = ray:hit_point()
	-- print("hit on object", hit_point.x, hit_point.y, hit_point.z)

	local techno_pressedon = Helpers.toTechno(ray:getFirstObject().attachedToObject).ExtTable
	TECHNO_SELECTED = techno_pressedon
end

function apply_key_command_to(self, keycode)
	local code = tonumber(Enums.KeyString[keycode])
	if code ~= nil then
		CURRENT_TERRAIN_TILE = code
	end

	if keycode == Enums.Key.S then
		if self.Techno:WhatAmI() == Enums.RTTITypeID.Techno then
			local _self = Utility.toTechno(self.Techno)
			print 'braking'
			_self.Physics.vehicle:brake_tyre_atonce(2, 1000)
			_self.Physics.vehicle:brake_tyre_atonce(3, 1000)
		end
	end
	if keycode == Enums.Key.D then
		if self:WhatAmI() == Enums.RTTITypeID.Techno then
			local _self = Utility.toTechno(self)
			-- _self.Physics:applyCentralImpulse_Directional(-1000)
			util.getObjectTable(_self).MovingState_Reverse = true
		end
	end
end

function InputHandler.KeyPress(keycode)
	apply_key_command_to(TECHNO_SELECTED, keycode)
end

function InputHandler.MouseMove(mouse_status)
	if mouse_status.left then
		local coord = Utility.GetCoordFromPoint(mouse_status.pos)
		local rt_start = Utility.CoordStruct(coord.x-600, coord.y-600, coord.z-500)
		local rt_end = Utility.CoordStruct(coord.x+6000, coord.y+6000, coord.z+5000)
		local ray_cell = Physics.RayTestSingleForCell.createRayTestForCell(rt_end, rt_start)
		ray_cell:perform()
		if ray_cell:hit() then
			local hit_point = ray_cell:hit_point()
			Wonderland.set_texture(TERRAIN_CHUNKS[1], CURRENT_TERRAIN_TILE, hit_point)
			Wonderland.buffer_update(TERRAIN_CHUNKS[1])
		end
	end
end

-- called when mouse pressed on an empty cell
-- args: mouse_status: same as Functions.Session_MousePress
function InputHandler.MousePress_OnCell(mouse_status)
	local coord = Utility.GetCoordFromPoint(mouse_status.pos)
	local rt_start = Utility.CoordStruct(coord.x-600, coord.y-600, coord.z-500)
	local rt_end = Utility.CoordStruct(coord.x+6000, coord.y+6000, coord.z+5000)
	local ray_cell = Physics.RayTestSingleForCell.createRayTestForCell(rt_end, rt_start)
	ray_cell:perform()

	if ray_cell:hit() then
		local hit_point = ray_cell:hit_point()
		Wonderland.set_texture(TERRAIN_CHUNKS[1], CURRENT_TERRAIN_TILE, hit_point)
		Wonderland.buffer_update(TERRAIN_CHUNKS[1])

		if TECHNO_SELECTED then
			local Techno = Helpers.Techno_TechnoRTTIIDTable(TECHNO_SELECTED)
			local path, pathway = grit_find_path({ Techno:GetCoord().x, Techno:GetCoord().y, Techno:GetCoord().z }, { hit_point.x, hit_point.y, hit_point.z })
			TECHNO_SELECTED.components.a['SteeringLocomotor_Raycast']:move_path(path, pathway)
		end
	end

	-- ModEnvironment.Functions.createTechno(OBJECTS.TESTTECHNO, Utility.CoordStruct(coord.x, coord.y, 128), true)
end

function InputHandler.MousePress_Right(mouse_status)
	local coord = Utility.GetCoordFromPoint(mouse_status.pos)
	local rt_start = Utility.CoordStruct(coord.x-600, coord.y-600, coord.z-500)
	local rt_end = Utility.CoordStruct(coord.x+6000, coord.y+6000, coord.z+5000)
	local ray_cell = Physics.RayTestSingleForCell.createRayTestForCell(rt_end, rt_start)
	ray_cell:perform()

	if ray_cell:hit() then
		local hit_point = ray_cell:hit_point()
		Wonderland.blend_cells_batch(TERRAIN_CHUNKS[1], hit_point)
		Wonderland.buffer_update(TERRAIN_CHUNKS[1])
		print 'blending cells'
	end

end

return InputHandler