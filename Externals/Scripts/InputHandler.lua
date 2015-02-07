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
		-- print("hit on cell", hit_point.x, hit_point.y, hit_point.z)

		if TECHNO_SELECTED then
			Helpers.Techno_TechnoRTTIIDTable(TECHNO_SELECTED).Physics.vehicle:launch()
		end
	end

	ModEnvironment.Functions.createTechno(OBJECTS.TESTTECHNO, Utility.CoordStruct(coord.x, coord.y, 128), true)
end

return InputHandler