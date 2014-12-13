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
	local techno_pressedon = Helpers.toTechno(ray:getFirstObject().attachedToObject).ExtTable
	techno_pressedon.components.a['GraphVehicle']:select_single()
end

-- called when mouse pressed on an empty cell
-- args: mouse_status: same as Functions.Session_MousePress
function InputHandler.MousePress_OnCell(mouse_status)
	local coord = Utility.GetCoordFromPoint(mouse_status.pos)
	local rt_start = coord
	local rt_end = Utility.CoordStruct(coord.x+6000, coord.y+6000, coord.z+5000)
	local ray_cell = Physics.RayTestSingleForCell.createRayTestForCell(rt_end, rt_start)
	ray_cell:perform()

	if ray_cell:hit() then
		local hit_point = ray_cell:hit_point()
		print(hit_point.x, hit_point.y, hit_point.z)

		local nearest_node = find_nearest_node(hit_point, 64)
		if nearest_node and TECHNO_SELECTED then
			local idx_node = nearest_node.components.a['GraphNodeStore']:get_datafield 'idx_initial'
			print('Selected node ', idx_node)
			move_techno_graph(TECHNO_SELECTED, nearest_node)
		end
	end

end

return InputHandler